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

#ifndef __CS_BSPBBOX_H__
#define __CS_BSPBBOX_H__

#include "csgeom/math3d.h"
#include "csgeom/polyidx.h"
#include "csgeom/poly3d.h"
#include "csgeom/transfrm.h"
#include "csengine/polyint.h"
#include "csengine/pol2d.h"
#include "csengine/treeobj.h"
#include "csobject/csobject.h"

class csPolyTreeBBox;
class csPolygonTree;
class csTransform;
class csBox3;

/**
 * A factor for creating instances of csBspPolygon.
 */
class csBspPolygonFactory : public csPolygonIntFactory
{
  /// Create a csBspPolygon.
  virtual csPolygonInt* Create ();
  /// Initialize a csBspPolygon.
  virtual void Init (csPolygonInt* pi);
};

/**
 * This class represents a polygon which can be inserted dynamically
 * in a BSP tree. It is specifically designed to be able to add bounding
 * boxes to sprites and dynamic things and then add those bounding boxes
 * to the BSP tree.
 */
class csBspPolygon : public csPolygonInt
{
  friend class csBspPolygonFactory;

private:
  /// The 3D polygon.
  csPolyIndexed polygon;
  /// The plane.
  csPlane3 plane;
  /// The parent.
  csPolyTreeBBox* parent;
  /// The original object for which this polygon is a bounding box polygon.
  csObject* originator;

public:
  /// A pool of csBspPolygon.
  static csPolygonIntPool& GetPolygonPool();

  /// Debug.
  void Dump();

private:
  /// Constructor is private to prevent allocation by non-factory.
  csBspPolygon () { }

public:
  /// Destructor.
  virtual ~csBspPolygon () { }

  /// Get the parent container.
  csPolyTreeBBox* GetParent () { return parent; }

  /// Set the parent container.
  void SetParent (csPolyTreeBBox* par) { parent = par; }

  /**
   * Get the original object for which this polygon is a bounding box polygon.
   */
  csObject* GetOriginator () { return originator; }

  /// Set the original object.
  void SetOriginator (csObject* orig) { originator = orig; }

  /// Get the reference to the polygon.
  csPolyIndexed& GetPolygon () { return polygon; }

  /// Get number of vertices.
  virtual int GetNumVertices () { return polygon.GetNumVertices (); }

  /// Get vertex index table (required for csPolygonInt).
  virtual int* GetVertexIndices () { return polygon.GetVertexIndices (); }

  /// Get original polygon (not known for this polygon type).
  virtual csPolygonInt* GetUnsplitPolygon () { return NULL; }

  /// Set the plane for this polygon.
  void SetPolyPlane (const csPlane3& pl) { plane = pl; }

  /// Return the plane of this polygon.
  csPlane3* GetPolyPlane () { return &plane; }

  /// Classify a polygon with regards to this one.
  int Classify (const csPlane3& pl);

  /// Same as Classify() but for X plane only.
  int ClassifyX (float x);

  /// Same as Classify() but for Y plane only.
  int ClassifyY (float y);

  /// Same as Classify() but for Z plane only.
  int ClassifyZ (float z);

  /// Split this polygon with the given plane (A,B,C,D).
  void SplitWithPlane (csPolygonInt** front, csPolygonInt** back,
  	const csPlane3& split_plane);

  /// Split this polygon to the x-plane.
  void SplitWithPlaneX (csPolygonInt** front, csPolygonInt** back, float x);

  /// Split this polygon to the y-plane.
  void SplitWithPlaneY (csPolygonInt** front, csPolygonInt** back, float y);

  /// Split this polygon to the z-plane.
  void SplitWithPlaneZ (csPolygonInt** front, csPolygonInt** back, float z);

  /// Return 3 to indicate it is a bsp polygon.
  int GetType () { return 3; }

  /// Transform the plane of this polygon.
  void Transform (const csTransform& trans);

  /**
   * Clip this polygon to the Z plane and if portal_plane is given also
   * clip the polygon to that plane.
   * @@@ NOTE @@@ This function is almost identical to the one in
   * csPolygon3D. It should be possible to reuse that code.
   */
  bool ClipToPlane (csPlane3* portal_plane, const csVector3& v_w2c,
	csVector3*& pverts, int& num_verts, bool cw = true);

  /**
   * Perspective correct a polygon. Note that this function is nearly
   * identical to a version in csPolygon3D. It should be possible to
   * reuse that code.
   */
  bool DoPerspective (const csTransform& trans,
	csVector3* source, int num_verts, csPolygon2D* dest, bool mirror);

  /**
   * Not implemented yet! @@@
   */
  bool Overlaps (csPolygonInt* /*overlapped*/) { return false; }
};

/**
 * A container for a bunch of BSP polygons.
 */
class csPolyTreeBBox : public csDetailedPolyTreeObject
{
private:
  /// Array of vertices.
  csVector3Array vertices;
  /// Array of camera space vertices.
  csVector3Array cam_vertices;

  /// All polygons.
  csPolygonStub* base_stub;

  /**
   * If true then this object is transformed to camera space.
   * i.e. cam_vertices will be valid.
   */
  bool is_cam_transf;

public:
  /// A factory for polygon stubs.
  static csPolygonStubFactory stub_fact;

public:
  /// Constructor.
  csPolyTreeBBox (csObject* owner);
  /// Destructor.
  virtual ~csPolyTreeBBox ();

  /// Get the base set of polygons.
  virtual csObjectStub* GetBaseStub () { return base_stub; }

  /// Get vector array for this container.
  csVector3Array& GetVertices () { return vertices; }
  
  /// Get camera vector array for this container.
  csVector3Array& GetCameraVertices () { return cam_vertices; }

  /**
   * Update this object using an object space bounding box
   * and a transformation.
   */
  void Update (const csBox3& object_bbox, const csTransform& o2w,
  	csObject* originator);

  /**
   * Update this object using a world space bounding box.
   */
  void Update (const csBox3& world_bbox,
  	csObject* originator);

  /// Add a polygon to this container.
  void AddPolygon (csPolygonInt* poly)
  {
    base_stub->GetPolygonArray ().AddPolygon (poly);
    ((csBspPolygon*)poly)->SetParent (this);
  }

  /// Get the number of polygons in this polygonset.
  int GetNumPolygons () { return base_stub->GetNumPolygons (); }

  /// Get the specified polygon from this set.
  csPolygonInt* GetPolygon (int idx)
  {
    return base_stub->GetPolygonArray ().GetPolygon (idx);
  }

  /// Get the array of polygons.
  csPolygonInt** GetPolygons () { return base_stub->GetPolygons (); }

  /// Transform the vertices of this container from world to camera.
  void World2Camera (const csTransform& trans);

  /// Return true if this object is already transformed to camera space.
  bool IsTransformed () { return is_cam_transf; }

  /// Clear camera transformation.
  void ClearTransform () { is_cam_transf = false; }
};

/**
 * An object stub that is used together with csSphereTreeObject.
 */
class csSphereStub : public csObjectStub
{
public:
  /// Visit this stub while traversing the tree (octree or BSP tree).
  virtual void* Visit (csSector* sector, csTreeVisitFunc* func, void* data);

  /// Initialize this stub.
  virtual void Initialize () { }

  /// Clean up data from this stub.
  virtual void RemoveData () { }
};

/**
 * Specific implementation of the csObjectStubFactory for csSphereStub.
 */
class csSphereStubFactory : public csObjectStubFactory
{
public:
  /// Create a new stub.
  virtual csObjectStub* Create ()
  {
    return new csSphereStub ();
  }
};

/**
 * An object with a radius for inclusion in an octree.
 */
class csSphereTreeObject : public csPolyTreeObject
{
private:
  /// The radius.
  float radius;
  /// The center point.
  csVector3 center;
  /// The stub.
  csSphereStub* base_stub;

public:
  /// The factory for radius stubs.
  static csSphereStubFactory stub_fact;
  /// Pool for sphere stubs.
  static csObjectStubPool stub_pool;

public:
  /// Constructor.
  csSphereTreeObject (csObject* owner, const csVector3& center, float rad);
  /// Destructor.
  virtual ~csSphereTreeObject () { }

  /// Get the base set of polygons.
  virtual csObjectStub* GetBaseStub () { return base_stub; }

  /// Split with plane.
  virtual void SplitWithPlane (csObjectStub* stub,
  	csObjectStub** stub_on, csObjectStub** stub_front,
	csObjectStub** stub_back,
	const csPlane3& plane);

  /// Split with plane.
  virtual void SplitWithPlaneX (csObjectStub* stub,
  	csObjectStub** stub_on, csObjectStub** stub_front,
	csObjectStub** stub_back,
	float x);

  /// Split with plane.
  virtual void SplitWithPlaneY (csObjectStub* stub,
  	csObjectStub** stub_on, csObjectStub** stub_front,
	csObjectStub** stub_back,
	float y);

  /// Split with plane.
  virtual void SplitWithPlaneZ (csObjectStub* stub,
  	csObjectStub** stub_on, csObjectStub** stub_front,
	csObjectStub** stub_back,
	float z);
};

#endif // __CS_BSPBBOX_H__
