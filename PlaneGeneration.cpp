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

FT plane_function(Point_3 p)
{
  const Point_3 n(0.0, 0.0, 1.0); // plane normal
  return n.x() * p.x() + n.y() * p.y() + n.z() * p.z();
}

void GeneratePlaneMesh()
{
  Tr tr;
  C2t3 c2t3_plane(tr);

  Surface_3 plane_surface(plane_function, Sphere_3(CGAL::ORIGIN, 2.0));

  CGAL::Surface_mesh_default_criteria_3<Tr> criteria(15.0, 0.1, 0.1);

  CGAL::make_surface_mesh(c2t3_plane, plane_surface, criteria, CGAL::Non_manifold_tag());

  Surface_mesh sm;
  CGAL::facets_in_complex_2_to_triangle_mesh(c2t3_plane, sm);
//  CGAL::refine_Delaunay_mesh_2(sm, CGAL::Surface_mesh_default_criteria_3<Tr>(0.125, 0.5));
  std::ofstream out("plane.off");
  out << sm << std::endl;

  std::cout << "Final number of points: " << tr.number_of_vertices() << "\n";
}