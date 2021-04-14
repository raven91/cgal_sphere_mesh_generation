//
// Created by Nikita Kruk on 14.04.21.
//

#include <fstream>

#include <CGAL/Surface_mesh_default_triangulation_3.h>
#include <CGAL/Complex_2_in_triangulation_3.h>
#include <CGAL/make_surface_mesh.h>
#include <CGAL/Implicit_surface_3.h>
#include <CGAL/IO/facets_in_complex_2_to_triangle_mesh.h>
#include <CGAL/Surface_mesh.h>

// default triangulation for Surface_mesher
typedef CGAL::Surface_mesh_default_triangulation_3 Tr;

// c2t3
typedef CGAL::Complex_2_in_triangulation_3<Tr> C2t3;

typedef Tr::Geom_traits GT;
typedef GT::Sphere_3 Sphere_3;
typedef GT::Point_3 Point_3;
typedef GT::FT FT;

typedef FT (*Function)(Point_3);

typedef CGAL::Implicit_surface_3<GT, Function> Surface_3;

typedef CGAL::Surface_mesh<Point_3> Surface_mesh;

FT ellipsoid_function(Point_3 p)
{
  const double a = 2.0, b = 1.0, c = 1.0;
  const double k = 4;
  const FT x2 = std::pow(p.x() / a, k), y2 = std::pow(p.y() / b, k), z2 = std::pow(p.z() / c, k);

  return x2 + y2 + z2 - 1.0; // (x/a)^k + (y/b)^k + (z/c)^k - 1 = 0
}

void GenerateEllipsoidalMesh()
{
  Tr tr; // 3D-Delaunay triangulation
  C2t3 c2t3(tr); // 2d-complex in 3D-Delaunay triangulation

  // defining the surface
  Surface_3 surface(ellipsoid_function, // pointer to function
                    Sphere_3(CGAL::ORIGIN, 6.0)); // bounding sphere
  // Note that "2.0" above is the *squared* radius of the bounding sphere!
  // Should be >= a^2 + b^2 + c^2

  // defining  meshing criteria
  CGAL::Surface_mesh_default_criteria_3<Tr> criteria(30.0, // angular bound
                                                     0.15, // radius bound
                                                     0.1); // distance bound
  // meshing surface
  CGAL::make_surface_mesh(c2t3, surface, criteria, CGAL::Non_manifold_tag());

  Surface_mesh sm;
  CGAL::facets_in_complex_2_to_triangle_mesh(c2t3, sm);
  std::ofstream out("ellipsoid.off");
  out << sm << std::endl;

  std::cout << "Final number of points: " << tr.number_of_vertices() << "\n";
}