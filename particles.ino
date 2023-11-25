
/*
	A 3D Particles demo
	Based upon the original OpenGL GLFW Particles demo.
	
	Michael McE
*/



#include "common.h"



#define DWIDTH		TFT_WIDTH
#define DHEIGHT		TFT_HEIGHT

static _ufont_t font;
_ufont_surface_t *surface;



//#define FtoRGB24(r,g,b)	((int)((r)*255.0)<<16 | (int)((g)*255.0)<<8 | (int)((b)*255.0))
#define FtoRGB565(r,g,b)	((int)((r)*31.0f)<<11 | (int)((g)*63.0f)<<5 | (int)((b)*31.0f))

#ifndef DEGTORAD
#define DEGTORAD			(0.0349065f)
#endif
#define M_PIf				(3.1415926f)



static float xpos, ypos, zpos,angle_x, angle_y, angle_z;

const int destx		=		(DWIDTH>>1);
const int desty		=		(DHEIGHT>>1);
const float zoom	=		-60.0f; // camz
const float flength	=		100.0f;

// Speed factor
#define SPEED				1.5f

// Maximum number of particles
#define MAX_PARTICLES		(3000)

// Life span of a particle (in seconds)
#define LIFE_SPAN 		16.0f

// A new particle is born every [BIRTH_INTERVAL] second
#define BIRTH_INTERVAL (LIFE_SPAN/(float)MAX_PARTICLES)

// Particle size (meters)
#define PARTICLE_SIZE   4.0f

// Gravitational constant (m/s^2)
#define GRAVITY         9.8f

// Base initial velocity (m/s)
#define VELOCITY        12.0f

// Bounce friction (1.0 = no friction, 0.0 = maximum friction)
#define FRICTION        0.75f

// "Fountain" height (m)
#define FOUNTAIN_HEIGHT 30.0f

// Fountain radius (m)
#define FOUNTAIN_RADIUS 6.6f

// Minimum delta-time for particle phisics (s)
#define MIN_DELTA_T     (BIRTH_INTERVAL*5.0f)

typedef struct {
	float z;
	int16_t x;
	int16_t y;
	uint16_t c;
	int16_t active;
}TZPOINT;
static TZPOINT points[MAX_PARTICLES];

/*
typedef struct {
	int16_t x;
	int16_t y;
}TPOINTOLD;
static TPOINTOLD old[MAX_PARTICLES];
*/

// This structure holds all state for a single particle
typedef struct {
    float x,y,z;     // Position in space
    float vx,vy,vz;  // Velocity vector
    float r,g,b;     // Colour of particle
    float life;      // Life of particle (1.0 = newborn, < 0.0 = dead)
    int   active;    // Tells if this particle is active
}PARTICLE;

// Global vectors holding all particles. We use two vectors for float
// buffering.
static PARTICLE particles[MAX_PARTICLES];

// Global variable holding the age of the youngest particle
static float min_age = 0.0f;





static void rotateX (const float angle, const float y, const float z, float *yr, float *zr)
{
	*yr = y * cosf(angle) - z * sinf(angle);
	*zr = y * sinf(angle) + z * cosf(angle);
}

static void rotateY (const float angle, const float x, const float z, float *xr, float *zr)
{
	*xr =  x * cosf(angle) + z * sinf(angle);
	*zr = -x * sinf(angle) + z * cosf(angle);
}

static void rotateZ (const float angle, const float x, const float y, float *xr, float *yr)
{
	*xr = x * cosf(angle) - y * sinf(angle);
	*yr = x * sinf(angle) + y * cosf(angle);
}

static void point3DTo2D (const float x, const float y, const float z, const float flength, const float camz, const int x0, const int y0, int *screenx, int *screeny)
{
	if (z-camz < 0.0f) return;
	
	const float tmp = flength/(z-camz);
	*screenx = (x*tmp)+x0;
	*screeny = (y*tmp)+y0;
}

static void vertToPoint (float xx, float yy, float zz, int *x1, int *y1)
{
	const float ThetaX = -53.0f * DEGTORAD; //xpos * DEGTORAD;
	const float ThetaY = ypos * DEGTORAD;
	const float ThetaZ = zpos * DEGTORAD;
		
//	float xx = x;
//	float yy = y;
//	float zz = z;
	rotateX(ThetaX, yy, zz, &yy, &zz);
	rotateY(ThetaY, xx, zz, &xx, &zz);
	rotateZ(ThetaZ, xx, yy, &xx, &yy);
	point3DTo2D(xx, yy, zz, flength, zoom, destx, desty, x1, y1);
	
	*y1 = (DHEIGHT-1) - *y1;
	//*x1 = (DWIDTH-1) - *x1;
}

static void InitParticle (PARTICLE *p, const float t)
{

    // Start position of particle is at the fountain blow-out
    p->x = 0.0f;
    p->y = 0.0f;
    p->z = FOUNTAIN_HEIGHT;

    // Start velocity is up (Z)...
    p->vz = 0.7f + (0.3f/4096.0f) * (float)(rand() & 4095);
    //p->vz = 0.7f + (0.3f/4096.0f) * (float)(random(4095));

    // ...and a randomly chosen X/Y direction
    float xy_angle = (2.0f*M_PIf/4096.0f) * (float)(rand() & 4095);
    //float xy_angle = (2.0f*M_PIf/4096.0f) * (float)(random(4095));
    p->vx = 0.4f * (float)cosf(xy_angle);
    p->vy = 0.4f * (float)sinf(xy_angle);

    // Scale velocity vector according to a time-varying velocity
    float velocity = VELOCITY*(0.8f + 0.1f*(float)(sinf(0.5f*t)+sinf(1.31f*t)));
    p->vx *= velocity;
    p->vy *= velocity;
    p->vz *= velocity;


    // colour is time-varying
    p->r = 0.7f + 0.3f * (float)sinf(0.34f*t + 0.1f);
    p->g = 0.6f + 0.4f * (float)sinf(0.63f*t + 1.1f);
    p->b = 0.6f + 0.4f * (float)sinf(0.91f*t + 2.1f);

    // The particle is new-born and active
    p->life = 1.0f;
    p->active = 1;
}

#define FOUNTAIN_R2 (FOUNTAIN_RADIUS+PARTICLE_SIZE/2)*(FOUNTAIN_RADIUS+PARTICLE_SIZE/2)

static void UpdateParticle (PARTICLE *p, const float dt)
{
    // If the particle is not active, we need not do anything
    if (!p->active) return;

    // The particle is getting older...
    p->life = p->life - dt * (1.0f / LIFE_SPAN);

    // Did the particle die?
    if (p->life <= 0.0f){
        p->active = 0;
        return;
    }

    // Update particle velocity (apply gravity)
    p->vz = p->vz - GRAVITY * dt;

    // Update particle position
    p->x = p->x + p->vx * dt;
    p->y = p->y + p->vy * dt;
    p->z = p->z + p->vz * dt;

    // Simple collision detection + response
    if (p->vz < 0.0f){
        // Particles should bounce on the fountain (with friction)
        if ((p->x*p->x + p->y*p->y) < FOUNTAIN_R2 && p->z < (FOUNTAIN_HEIGHT + PARTICLE_SIZE/2.0f)){
            p->vz = -FRICTION * p->vz;
            p->z  = FOUNTAIN_HEIGHT + PARTICLE_SIZE/2.0f + FRICTION * (FOUNTAIN_HEIGHT + PARTICLE_SIZE/2.0f - p->z);
        }else if (p->z < PARTICLE_SIZE/2.0f){ // Particles should bounce on the floor (with friction)
            p->vz = -FRICTION * p->vz;
            p->z  = PARTICLE_SIZE/2.0f + FRICTION * (PARTICLE_SIZE/2.0f - p->z);
        }

    }
}

void ParticleEngine (const float t, float dt)
{
    // update particles (iterated several times per frame if dt is too
    // large)
    while (dt > 0.0f){
        // Calculate delta time for this iteration
        float dt2 = dt < MIN_DELTA_T ? dt : MIN_DELTA_T;

        // Update particles
        for (int i = 0; i < MAX_PARTICLES; i++)
            UpdateParticle(&particles[i], dt2);

        // Increase minimum age
        min_age += dt2;

        // Should we create any new particle(s)?
        while (min_age >= BIRTH_INTERVAL){
            min_age -= BIRTH_INTERVAL;

            // Find a dead particle to replace with a new one
            for (int i = 0; i < MAX_PARTICLES; i++){
                if (!particles[ i ].active){
                    InitParticle(&particles[ i ], t + min_age);
                    UpdateParticle(&particles[ i ], min_age);
                    break;
                }
            }
        }
        // Decrease frame delta time
        dt -= dt2;
    }
}

int zpsort (const void *a, const void *b)
{
	const TZPOINT *p1 = (TZPOINT*)a;
	const TZPOINT *p2 = (TZPOINT*)b;

	return p1->z - p2->z;
	/*if (p1->z > p2->z)
		return 1;
	else
		return -1;*/
}

void DrawParticles (const float t, const float dt)
{
    int sx = 0, sy = 0;
    float x,y,z;
	float x2,y2,z2;
	float tmp;
    const PARTICLE *pptr = particles;
	const float ThetaX = -53.0f * DEGTORAD; //xpos * DEGTORAD;
	const float ThetaY = ypos * DEGTORAD;
	const float ThetaZ = zpos * DEGTORAD;
	const float cosx = cosf(ThetaX);
	const float cosy = cosf(ThetaY);
	const float cosz = cosf(ThetaZ);
	const float sinx = sinf(ThetaX);
	const float siny = sinf(ThetaY);
	const float sinz = sinf(ThetaZ);


    for (int i = MAX_PARTICLES; --i;){
    	//if (!points[i].active){
    		
        if (pptr->active){
    		//tft.fillCircle(points[i].x, points[i].y, PARTICLE_SIZE, 0);

			x = pptr->x;
			z = pptr->z;
			y = pptr->y;
			
  			//RotateX (ThetaX, &y, &z);
  			y2 = (y*cosx) - (z*sinx);
			z = (z*cosx) + (y*sinx);
			y = y2;
				
  			//RotateY (ThetaY, &x, &z);
  			z2 = (z*cosy) - (x*siny);
			x = (x*cosy) + (z*siny);
			z = z2;
				
   			//RotateZ (ThetaZ, &x, &y);
			x2 = (x*cosz) - (y*sinz);
			y = (y*cosz) + (x*sinz);
			x = x2;

			if (z-zoom >= 0.0f){
				tmp = flength/(z-zoom);
				sx = (x*tmp)+destx;
				sy = (-y*tmp)+desty;

				points[i].z = tmp;
				points[i].x = sx;
				points[i].y = sy;
				points[i].c = FtoRGB565(pptr->r, pptr->g, pptr->b);

				points[i].active = 0;
			}else{
				points[i].active = 1;
			}
        }
        pptr++;
    }
    
    // sort z so that near particles are rendered last
	qsort(points, MAX_PARTICLES, sizeof(TZPOINT), zpsort);

	
    /*
    // erase old
    for (int i = 0; i < MAX_PARTICLES; i++){
    	if (!points[i].active)
    		tft.fillCircle_writeA(old[i].x, old[i].y, PARTICLE_SIZE, 0);
    }*/


	tft_clearBuffer(COLOUR_BLACK);
	TZPOINT *p = points;
	_ufont_surface_t *dsurface = fontGetDisplayBuffer(&font);
		
    for (int i = 0; i < MAX_PARTICLES; i++, p++){
    	if (!p->active){
    		surfaceDrawCircleFilled(dsurface, p->x, p->y, PARTICLE_SIZE, p->c);
    		//printf("%i %i %i\r\n", p->x, p->y, p->c);
    		
    		//old[i].x = p->x;
    		//old[i].y = p->y;
    	}
    }
}

void DrawScene (const float t)
{
    static float t_old, dt;

    dt = t - t_old;
    t_old = t;


    // Rotate camera
    angle_x = 90.0f - 10.0f;
    angle_y = 40.0f * sinf(0.3f * t);
    angle_z = 10.0f * t;

    // Translate camera
    xpos =  15.0f * sinf((M_PIf/180.0f) * angle_z) + 2.0f * sinf((M_PIf/180.0f) * 3.1f * t);
    ypos = -15.0f * cosf((M_PIf/180.0f) * angle_z) + 2.0f * cosf((M_PIf/180.0f) * 2.9f * t);
    zpos = 4.0f + 2.0f * cosf((M_PIf/180.0f) * 4.9f * t);

#if 0
	// rotate the scene
    xpos = 9.0f * cosf(0.3f * t);
    ypos = 12.0f + 1.0f * sinf(0.3f * t);
    zpos = 9.0f * sinf(0.3f * t);
#endif

	ParticleEngine(t, dt);
    DrawParticles(t, dt);
}

void initTFT ()
{
	tft_init();		
	tft_clear(COLOUR_BLACK);
	tft_update();
}

void setup ()
{
	//Serial.begin(9600);
	//while (!Serial);
	//printf("uFont\r\n");

	initTFT();
	sd_init();

	if (!fontOpen(&font, NULL)){
		//printf("fontOpen() failed \r\n");
		return;
	}
	

	fontSetDisplayBuffer(&font, tft_getBuffer(), TFT_WIDTH, TFT_HEIGHT);
	
	// create a 1BPP work surface. this is the initial work surface
	// we don't use this as we're writtign directly to the 16bit front buffer
	// surface = fontCreateSurface(DWIDTH, DHEIGHT, COLOUR_BLACK, NULL);
	
	// Usually we must do this, but as we know we arn't writting to this, its not required
	// fontSetRenderSurface(&font, surface);

	randomSeed(analogRead(A9));
			
	for (int i = 0; i < MAX_PARTICLES; i++)
		particles[i].active = 0;
}

void loop ()
{
	static uint32_t t0 = millis();
	

	DrawScene(((millis()-t0)/1000.0f)*SPEED);
	tft_update();
}


