# SUBLIMED

SUBlimated cometary gases in LIME (Dynamical version)

This is SUBLIMED, a 3D radiative transfer code for outflowing cometary comae, by Martin Cordiner and Miguel de Val Borro (2018). 

SUBLIMED is based on LIME (LIne Modeling Engine) version 1.9.3 by Christian Brinch (2006-2014) and the LIME development team (2015-2018). Main changes from the original LIME code include (1) switching from the static (GSL) solver to a time-dependent (CVODE) solver, which allows the dynamical nature of the cometary outflow to be properly simulated. Equations of statistical equilibrium in solver.c have been restructured as time-dependent differential equations, assuming constant outflow velocity. (2) Electron collision rates are added to the differential equations; analytic electron densities and temperatures are generated at runtime. (3) The raytracing routine in raytrace.c has been altered to provide sufficient (evenly-weighted) sampling in the central image pixels to account for the strong, compact, central brightness peak of the coma. (4) For photon trapping, the escape probability approximation has been implemented to allow faster runtime, invoked using par->useEP=1.

Example input and output files are given in the example/ folder.

After installation of the required gsl, qhull and cvode (SUNDIALS) libraries, recommend running the executable as follows:

> lime -f -n -p <#threads> model.c

To generate a reliable coma image, care needs to be taken to correctly set the par->radius parameter in model.c to capture all the expected emission (this will vary depending on the molecule, coma and viewing geometry). The channel spacing (velres) needs to be small enough (around 0.1 km/s or less) to properly sample the spectral line profile, even if the velocity information is later discarded. 

Experience has shown that a good model can be produced with par->pIntensity ~ 2000 grid points (and par->sinkPoints = 500 sink points). Radiation trapping effects tend to be very small, so useEP = 0 can be set for most models (apart from H2O), which allows the code to run much faster (in a matter of seconds), particularly for CH3OH.