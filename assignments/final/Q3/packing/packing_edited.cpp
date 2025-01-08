#include <iostream>
#include <vector>
#include <fstream>
#include <random>
#include <chrono>
#include <omp.h>

void pbc_loop(std::vector<double> &x,
              std::vector<double> &y,
              std::vector<double> &z,
              std::vector<double> &particle_radius,
              double &fx,
              double &fy,
              double &fz,
              const double offset_x,
              const double offset_y,
              const double offset_z,
              const double force_size,
              const double rix,
              const double riy,
              const double riz,
              const double rir)
{

   const int num_particles = x.size();

   // define function variables
   double rjx, rjy, rjz, rjr; // particle j positions/radii
   double dx, dy, dz, rr;     // position vector and radii
#pragma omp parallel default(shared)
   {
#pragma omp for reduction(+ : fx, fy, fz)
      for (int j = 0; j < num_particles; j++)
      {
         // ALl read-only so is thread-safe
         rjx = x[j] + offset_x;
         rjy = y[j] + offset_y;
         rjz = z[j] + offset_z;
         rjr = particle_radius[j];
         rr = (rir + rjr) * (rir + rjr);
         dx = rjx - rix;
         dy = rjy - riy;
         dz = rjz - riz;
         // check if particles overlap
         if (dx * dx + dy * dy + dz * dz < rr)
         {
            fx += -force_size * dx;
            fy += -force_size * dy;
            fz += -force_size * dz;
         }
      }
   }
}

void calculate_forces(std::vector<double> &x,
                      std::vector<double> &y,
                      std::vector<double> &z,
                      std::vector<double> &particle_radius,
                      std::vector<double> &force_x,
                      std::vector<double> &force_y,
                      std::vector<double> &force_z,
                      double Lx,
                      double Ly,
                      double Lz)
{

   const int np = x.size();
   const double force_size = 0.1;

   // define function variables
   double rix, riy, riz, rir;         // particle j positions/radii
   double fx, fy, fz;                 // force components
   bool bpx, bmx, bpy, bmy, bpz, bmz; // boolean tests

   // loop over all particles and calculate total force
   for (int i = 0; i < np; i++)
   {
      riz = z[i];
      rix = x[i];
      riy = y[i];
      rir = particle_radius[i];
      fx = 0.0;
      fy = 0.0;
      fz = 0.0;

      // internal interactions
      pbc_loop(x, y, z, particle_radius, fx, fy, fz, 0.0, 0.0, 0.0, force_size, rix, riy, riz, rir);

      bpx = rix > Lx - 1.1 * rir;
      bmx = rix < 1.1 * rir;
      bpy = riy > Ly - 1.1 * rir;
      bmy = riy < 1.1 * rir;
      bpz = riz > Lz - 1.1 * rir;
      bmz = riz < 1.1 * rir;

      // now check for boundary forces
      // in the plane
      if (bmx)
         pbc_loop(x, y, z, particle_radius, fx, fy, fz, -Lx, 0.0, 0.0, force_size, rix, riy, riz, rir); //-x,0,0
      if (bpx)
         pbc_loop(x, y, z, particle_radius, fx, fy, fz, Lx, 0.0, 0.0, force_size, rix, riy, riz, rir); //+x,0,0
      if (bpy)
         pbc_loop(x, y, z, particle_radius, fx, fy, fz, 0.0, Ly, 0.0, force_size, rix, riy, riz, rir); //+y,0,0
      if (bmy)
         pbc_loop(x, y, z, particle_radius, fx, fy, fz, 0.0, -Ly, 0.0, force_size, rix, riy, riz, rir); //-y,0,0
      if (bpx && bpy)
         pbc_loop(x, y, z, particle_radius, fx, fy, fz, Lx, Ly, 0.0, force_size, rix, riy, riz, rir); //+x,+y,0
      if (bpx && bmy)
         pbc_loop(x, y, z, particle_radius, fx, fy, fz, Lx, -Ly, 0.0, force_size, rix, riy, riz, rir); //+x,-y,0
      if (bmx && bpy)
         pbc_loop(x, y, z, particle_radius, fx, fy, fz, -Lx, Ly, 0.0, force_size, rix, riy, riz, rir); //-x,+y,0
      if (bmx && bmy)
         pbc_loop(x, y, z, particle_radius, fx, fy, fz, -Lx, -Ly, 0.0, force_size, rix, riy, riz, rir); //-x,-y,0

      // up
      if (bpz)
         pbc_loop(x, y, z, particle_radius, fx, fy, fz, 0.0, 0.0, Lz, force_size, rix, riy, riz, rir); // 0,0,+z
      if (bmx && bpz)
         pbc_loop(x, y, z, particle_radius, fx, fy, fz, -Lx, 0.0, Lz, force_size, rix, riy, riz, rir); //-x,0,+z
      if (bpx && bpz)
         pbc_loop(x, y, z, particle_radius, fx, fy, fz, Lx, 0.0, Lz, force_size, rix, riy, riz, rir); //+x,0,+z
      if (bpy && bpz)
         pbc_loop(x, y, z, particle_radius, fx, fy, fz, 0.0, Ly, Lz, force_size, rix, riy, riz, rir); //+y,0,+z
      if (bmy && bpz)
         pbc_loop(x, y, z, particle_radius, fx, fy, fz, 0.0, -Ly, Lz, force_size, rix, riy, riz, rir); //-y,0,+z
      if (bpx && bpy && bpz)
         pbc_loop(x, y, z, particle_radius, fx, fy, fz, Lx, Ly, Lz, force_size, rix, riy, riz, rir); //+x,+y,+z
      if (bpx && bmy && bpz)
         pbc_loop(x, y, z, particle_radius, fx, fy, fz, Lx, -Ly, Lz, force_size, rix, riy, riz, rir); //+x,-y,+z
      if (bmx && bpy && bpz)
         pbc_loop(x, y, z, particle_radius, fx, fy, fz, -Lx, Ly, Lz, force_size, rix, riy, riz, rir); //-x,+y,+z
      if (bmx && bmy && bpz)
         pbc_loop(x, y, z, particle_radius, fx, fy, fz, -Lx, -Ly, Lz, force_size, rix, riy, riz, rir); //-x,-y,+z

      // down
      if (bmz)
         pbc_loop(x, y, z, particle_radius, fx, fy, fz, 0.0, 0.0, -Lz, force_size, rix, riy, riz, rir); // 0,0,-z
      if (bmx && bmz)
         pbc_loop(x, y, z, particle_radius, fx, fy, fz, -Lx, 0.0, -Lz, force_size, rix, riy, riz, rir); //-x,0,-z
      if (bpx && bmz)
         pbc_loop(x, y, z, particle_radius, fx, fy, fz, Lx, 0.0, -Lz, force_size, rix, riy, riz, rir); //+x,0,-z
      if (bpy && bmz)
         pbc_loop(x, y, z, particle_radius, fx, fy, fz, 0.0, Ly, -Lz, force_size, rix, riy, riz, rir); //+y,0,-z
      if (bmy && bmz)
         pbc_loop(x, y, z, particle_radius, fx, fy, fz, 0.0, -Ly, -Lz, force_size, rix, riy, riz, rir); //-y,0,-z
      if (bpx && bpy && bmz)
         pbc_loop(x, y, z, particle_radius, fx, fy, fz, Lx, Ly, -Lz, force_size, rix, riy, riz, rir); //+x,+y,-z
      if (bpx && bmy && bmz)
         pbc_loop(x, y, z, particle_radius, fx, fy, fz, Lx, -Ly, -Lz, force_size, rix, riy, riz, rir); //+x,-y,-z
      if (bmx && bpy && bmz)
         pbc_loop(x, y, z, particle_radius, fx, fy, fz, -Lx, Ly, -Lz, force_size, rix, riy, riz, rir); //-x,+y,-z
      if (bmx && bmy && bmz)
         pbc_loop(x, y, z, particle_radius, fx, fy, fz, -Lx, -Ly, -Lz, force_size, rix, riy, riz, rir); //-x,-y,-z

      force_x[i] = fx;
      force_y[i] = fy;
      force_z[i] = fz;

   } // i-loop
}

void move_particles(std::vector<double> &x,
                    std::vector<double> &y,
                    std::vector<double> &z,
                    std::vector<double> &fx,
                    std::vector<double> &fy,
                    std::vector<double> &fz,
                    const double dt)
{

   const double mass = 1.0;
   const int np = x.size();

   // relaxation algorithm - simply move particles along direction of force
   for (int i = 0; i < np; i++)
   {
      x[i] = x[i] + fx[i] * dt;
      y[i] = y[i] + fy[i] * dt;
      z[i] = z[i] + fz[i] * dt;
   }

   return;
};

void shrink(std::vector<double> &x,
            std::vector<double> &y,
            std::vector<double> &z,
            double &L,
            double dL)
{

   const double shrink_factor = (L - dL) / L;
   const int np = x.size();

   // relaxation algorithm - simply move particles along direction of force
   for (int i = 0; i < np; i++)
   {
      x[i] = x[i] * shrink_factor;
      y[i] = y[i] * shrink_factor;
      z[i] = z[i] * shrink_factor;
   }

   // change box size
   L = L - dL;

   return;
}

// simple wrapper class for rng
class rng_t
{

   // std::random variables (internal to class)
   std::mt19937 mt; // mersenne twister
   std::uniform_real_distribution<double> dist;

public:
   // seed rng
   void seed(unsigned int random_seed)
   {
      dist = std::uniform_real_distribution<double>(0.0, 1.0); // uniform distribution [0:1)
      std::mt19937::result_type mt_seed = random_seed;
      mt.seed(mt_seed);
   }

   // generate a uniform random number between 0 and 1
   double grnd()
   {
      return dist(mt);
   }
};

// simple wrapper class for rng
class ln_rng_t
{

   // std::random variables (internal to class)
   std::mt19937 mt; // mersenne twister
   std::lognormal_distribution<double> dist;

public:
   // seed rng
   void seed(unsigned int random_seed, double mean, double standard_deviation)
   {
      const double m = log(mean);
      const double s = standard_deviation;
      dist = std::lognormal_distribution<double>(m, s); // uniform distribution [0:1)
      std::mt19937::result_type mt_seed = random_seed;
      mt.seed(mt_seed);
   }

   // generate a uniform random number between 0 and 1
   double grnd()
   {
      return dist(mt);
   }
};

int main()
{

   // define number of particles and layers
   const int num_particles = 4000;
   const double mean_volume = 200.0;

   // particle positions
   std::vector<double> x(num_particles);
   std::vector<double> y(num_particles);
   std::vector<double> z(num_particles);

   // particle volumes and radii
   std::vector<double> particle_volume(num_particles);
   std::vector<double> particle_radius(num_particles);

   const double ftpi = M_PI * 4.0 / 3.0;
   const double mean_radius = pow(mean_volume / ftpi, 0.33333333); // 3D
   const double volume_distribution = 0.3;

   std::cout << "Mean radius: " << mean_radius << std::endl;

   // initialise random number generators
   rng_t urng;
   urng.seed(37322822);
   ln_rng_t lnrng;
   lnrng.seed(27357328, mean_volume, volume_distribution);
   //------------------------------------------------------
   // Generate a lognormal ditribution of particle volumes
   //------------------------------------------------------
   for (int i = 0; i < num_particles; i++)
   {
      particle_volume[i] = lnrng.grnd();
      particle_radius[i] = pow(particle_volume[i] / ftpi, 0.33333333);
      // particle_radius[i] = pow(particle_volume[i]/(ftpi*thickness),0.5);
   }

   // std::cout << "here" << std::endl;
   //  calculate volume distribution
   std::vector<double> volume_bins(200);
   std::vector<double> radius_bins(200);

   for (int i = 0; i < num_particles; i++)
   {
      double V = particle_volume[i];
      int bin_ID = int((V + 0.5) / 10.0);
      if (bin_ID < 200)
         volume_bins[bin_ID] += 1.0;

      double r = particle_radius[i];
      int rbin_ID = int((r + 0.5) / 1.0);
      if (rbin_ID < 200)
         radius_bins[rbin_ID] += 1.0;
   }
   std::ofstream ofile("volumes.txt");
   for (int i = 0; i < 200; i++)
   {
      ofile << double(i) * 10.0 << "\t" << volume_bins[i] << "\t" << double(i) * 1.0 << "\t" << radius_bins[i] << std::endl;
   }
   ofile.close();

   // define 3D box size
   double L = 2.0 * pow(num_particles, 0.3333333333) * mean_radius; // diameter = 2r
   double L_init = L * 2.0;
   std::cout << "System size: " << L << std::endl;

   //----------------------------------------------
   // generate random positions of particles
   //----------------------------------------------
   for (int i = 0; i < num_particles; i++)
   {
      x[i] = L_init * urng.grnd();
      y[i] = L_init * urng.grnd();
      z[i] = L_init * urng.grnd();
   }

   //----------------------------------------------
   // relax particles
   //----------------------------------------------
   std::vector<double> fx(num_particles);
   std::vector<double> fy(num_particles);
   std::vector<double> fz(num_particles);

   const int num_steps = 2000;
   const double L_final = L;
   const double dL = (L_init - L_final) / double(num_steps);
   double dt = 0.1 * mean_radius;

   // coarse relaxation
   L = L_init;
   for (int t = 0; t < num_steps; t++)
   {
      calculate_forces(x, y, z, particle_radius, fx, fy, fz, L, L, L);
      move_particles(x, y, z, fx, fy, fz, dt);
      shrink(x, y, z, L, dL);
      // std::cout << L << std::endl;
   }

   // fine relaxation
   dt = 0.05 * mean_radius;
   for (int t = 0; t < num_steps; t++)
   {
      calculate_forces(x, y, z, particle_radius, fx, fy, fz, L, L, L);
      move_particles(x, y, z, fx, fy, fz, dt);
   }

   ofile.open("final_positions.txt");
   for (int i = 0; i < num_particles; i++)
   {
      ofile << x[i] << "\t" << y[i] << "\t" << z[i] << "\t" << particle_radius[i] << std::endl;
   }
   ofile.close();

   double vol_spheres = 0.0;
   for (int i = 0; i < particle_volume.size(); i++)
      vol_spheres += particle_volume[i];

   std::cout << "Final box size: " << L << std::endl;
   std::cout << "Box size change: " << dL << std::endl;
   std::cout << "Final density: " << vol_spheres / (L * L * L) << std::endl;

   return 0;
}
