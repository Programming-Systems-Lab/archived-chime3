/*
  Copyright (C) 1998 by Jorrit Tyberghein

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Library General Public
  License as published by the Free Software Foundation; either
  version 2 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Library General Public License for more details.

  You should have received a copy of the GNU Library General Public
  License along with this library; if not, write to the Free
  Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#ifndef __FRUSTRUM_H__
#define __FRUSTRUM_H__

#include "cstypes.h"
#include "csgeom/math3d.h"

class csTransform;

/**
 * Return values for csFrustum::Classify. The routine makes a difference
 * whenever a polygon is fully outside the frustum, fully inside, fully
 * covers the frustum or is partly inside, partly outside.
 */
/// The polygon is fully outside frustum
#define CS_FRUST_OUTSIDE	0
/// The polygon is fully inside frustum
#define CS_FRUST_INSIDE		1
/// The polygon fully covers the frustum
#define CS_FRUST_COVERED	2
/// The polygon is partially inside frustum
#define CS_FRUST_PARTIAL	3

/**
 * A general frustum. This consist of a center point (origin),
 * a frustum polygon in 3D space (relative to center point)
 * and a plane. The planes which go through the center and
 * every edge of the polygon form the frustum. The plane
 * is the back plane of the frustum.
 * It is also possible to have an infinite frustum in which
 * case the polygon will be NULL (not specified). The back
 * plane can also be NULL.
 */
class csFrustum
{
private:

  /// The origin of this frustum
  csVector3 origin;

  /**
   * The polygon vertices for non-wide frustum.
   * If not NULL, the frustum is a pyramid with origin in "origin"
   * and with the basis given by this polygon.
   */
  csVector3* vertices;
  /// Number of vertices in frustum polygon
  int num_vertices;
  /// Max number of vertices
  int max_vertices;

  /// Back clipping plane
  csPlane3* backplane;

  /**
   * If true we have a total wide frustum. A frustum like
   * this does not have a polygon defining the planes but it can have
   * a back plane. The value of this boolean is only used if there
   * is no polygon. In other words, if the polygon is present in this
   * frustum then 'wide' is simply ignored and can have any value.
   */
  bool wide;

  /**
   * If true we have a mirrored frustum where the direction of
   * the polygon is reversed.
   */
  bool mirrored;

  /// The reference count for this frustum
  int ref_count;

  /// Clear the frustum
  void Clear ();

  /// Ensure vertex array is able to hold at least "num" vertices
  void ExtendVertexArray (int num);

public:
  /// Create a new empty frustum.
  csFrustum (const csVector3& o) : origin (o), vertices (NULL),
    num_vertices (0), max_vertices (0), backplane (NULL),
    wide (false), mirrored (false), ref_count (1)
  { }

  /**
   * Create a frustum given a polygon and a backplane.
   * The polygon is given relative to the origin 'o'.
   * If the given polygon is NULL then we create an empty frustum.
   */
  csFrustum (const csVector3& o, csVector3* verts, int num_verts, csPlane3* backp = NULL);

  /// Copy constructor.
  csFrustum (const csFrustum &copy);

  ///
  virtual ~csFrustum ();

  /// Set the origin of this frustum.
  void SetOrigin (csVector3& o) { origin = o; }

  /// Get the origin of this frustum.
  csVector3& GetOrigin () { return origin; }

  /**
   * Enable/disable mirroring.
   * If mirroring is enabled this means that the frustum polygon
   * is given in anti-clockwise order.
   */
  void SetMirrored (bool m) { mirrored = m; }

  /// Is this frustum mirrored?
  bool IsMirrored () { return mirrored; }

  /**
   * Set the back plane of this frustum.
   * The given plane is copied to this structure and can thus
   * be reused/freed later. The plane should be specified relative
   * to the origin point.
   */
  void SetBackPlane (csPlane3& plane);

  /**
   * Get the back plane.
   */
  csPlane3* GetBackPlane () { return backplane; }

  /**
   * Remove the back plane of this frustum.
   */
  void RemoveBackPlane ();

  /**
   * Add a vertex to the frustum polygon.
   */
  void AddVertex (const csVector3& v);

  /**
   * Get the number of vertices.
   */
  int GetNumVertices () { return num_vertices; }

  /**
   * Get a vertex.
   */
  csVector3& GetVertex (int idx) { return vertices[idx]; }

  /**
   * Get the array of vertices.
   */
  csVector3* GetVertices () { return vertices; }

  /**
   * Apply a transformation to this frustum.
   */
  void Transform (csTransform* trans);

  /**
   * Clip this frustum to the positive side of a plane
   * formed by the origin of this frustum, and the two given vertices.
   * 'v1' and 'v2' are given relative to that origin.
   */
  void ClipToPlane (csVector3& v1, csVector3& v2);

  /**
   * Clip the polygon of this frustum to the postive side of an arbitrary plane
   * (which should be specified relative to the origin of the frustum).
   * Note that this clips the polygon which forms the frustum. It does
   * not clip the frustum itself.
   */
  void ClipPolyToPlane (csPlane3* plane);

  /**
   * Intersect with another frustum. The other frustum
   * must have the same origin as this one. Otherwise the
   * result is undefined.
   * Returns new frustum which you should delete
   * after usage. If there is no intersection this function
   * returns NULL.
   */
  csFrustum* Intersect (const csFrustum& other);

  /**
   * Intersect a convex polygon with this volume. The convex polygon
   * is given relative to the center point (origin) of this frustum.<p>
   *
   * Returns a new frustum which exactly covers the intersection
   * of the polygon with the frustum (i.e. the smallest frustum
   * which is part of this frustum and which 'sees' exactly
   * the same of the given polygon as this frustum).<p>
   *
   * This function returns NULL if there is no intersection.<p>
   *
   * Note that the frustum polygon of the returned csFrustum is
   * guaranteed to be coplanar with the given polygon.
   */
  csFrustum* Intersect (csVector3* poly, int num);

  /**
   * Check if a polygon intersects with the frustum (i.e.
   * is visible in the frustum). Returns one of CS_FRUST_XXX values.
   * Frustum and polygon should be given relative to (0,0,0).
   */
  static int Classify (csVector3* frustum, int num_frust,
    csVector3* poly, int num_poly);

  /**
   * This is like the above version except that it takes a vector of precalculated frustum plane normals.
   * Use this if you have to classify a batch of polygons against the same frustum.
   */
  static int BatchClassify (csVector3* frustum, csVector3* frustumNormals, int num_frust,
			    csVector3* poly, int num_poly);

  /**
   * Check if a point (given relative to the origin of the frustum)
   * is inside the frustum.
   */
  bool Contains (const csVector3& point);

  /**
   * Check if a point is inside a frustum. The point and
   * frustum are relative to (0,0,0). Note that this function
   * does not work correctly if the point is in the other direction
   * from the average direction of the frustum.
   */
  static bool Contains (csVector3* frustum, int num_frust,
    const csVector3& point);

  /**
   * Check if a point is inside a frustum. The point and
   * frustum are relative to (0,0,0). This function also
   * checks if point is in front of given plane.
   */
  static bool Contains (csVector3* frustum, int num_frust,
    const csPlane3& plane, const csVector3& point);

  /// Return true if frustum is empty.
  bool IsEmpty () const { return !wide && vertices == NULL; }

  /// Return true if frustum is infinite.
  bool IsInfinite () const { return wide && vertices == NULL && backplane == NULL; }

  /// Return true if frustum is infinitely wide but it can still have a back plane.
  bool IsWide () const { return wide && vertices == NULL; }

  /**
   * Make the frustum infinite (i.e. clear the polygon and
   * the back plane).
   */
  void MakeInfinite ();

  /**
   * Make the frustum empty.
   */
  void MakeEmpty ();

  /// Increment reference counter
  void IncRef () { ref_count++; }
  /// Decrement reference counter
  void DecRef () { if (!--ref_count) delete this; }
};

#endif // __CSFRUSTRUM_H__
