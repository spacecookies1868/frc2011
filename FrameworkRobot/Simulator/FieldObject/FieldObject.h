/*
 *  FieldObject.h
 *  RoboSimOSX
 *
 *  Created by ebakan on 1/1/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef FIELD_OBJECT_H
#define FIELD_OBJECT_H

#include "FieldObject/Components/ConvexPolyhedron.h"
#include "Shared/Vector3.h"
#include "SimulatedRobot/SimModules/SimModuleBase.h"
#include <vector>
#include <string>

/**
 * Class for any object on the field.
 * @author Eric Bakan
 */
class FieldObject
{
public:

	/**
     * Unique identifier for type of FieldObject.
     */
	enum UID {
		fieldobject=0,
		robot,
		alliancewall,
		guardrail,
		lane,
		lanedivider,
		scoringgrid,
		scoringzone,
		tower,
		tube=10,
		tubering,
		tubesquare,
		tubetriangle
	};

	/**
	 * The alliance the FieldObject belongs to.
	 */
	enum Alliance {
		neutral=0,
		blue,
		red
	};

	/**
	 * Creates a FieldObject, default constructor.
	 */
	FieldObject();
	FieldObject(Vector3 pos, Vector3 angle, Alliance alliance, std::string descriptor);
	FieldObject(ConvexPolyhedron polyhedron, Vector3 pos, Vector3 angle, Alliance alliance, std::string descriptor);

	/**
	 * Destructor of a FieldObject.
     */
	virtual ~FieldObject();
	
	/**
	 * Checks if the FieldObjects are colliding.
	 * @param rhs the other FieldObject to check if colliding
	 * @return whether this object and the other are colliding
	 */
	bool IsIntersecting(FieldObject& rhs); //checks if FieldObjects are colliding by each's definition
	
	/**
	 * Prints a string indicating that this has collided with another FieldObject.
	 * @param rhs the object with which this has collided
	 */
	virtual void HandleCollision(FieldObject& rhs);
	
	/**
	 * Returns this object's polyhedra.
	 * @return this object's polyhedra.
	 */
	const std::vector<ConvexPolyhedron>& GetPolyhedra() {return m_polyhedra;}
	
	/**
	 * Returns the absolute values for position and angle of the polyhedra.
	 */
	const std::vector<ConvexPolyhedron>& GetAbsolutePolyhedra();
	
	//basic getters
	const Vector3& GetPos() const {return m_pos;}
	const Vector3& GetAngle() const {return m_angle;}
	const Vector3& GetVel() const {return m_vel;}
	const Vector3& GetRotVel() const {return m_rotvel;}
	const std::string& GetDescriptor() const {return m_descriptor;}
	const std::vector<Vector3>& GetRelativePositions() {return m_relativeposs;};
	const std::vector<Vector3>& GetRelativeAngles() {return m_relativeangles;};
	const bool& IsStatic() {return m_static;}
	const UID& GetUID() {return m_uid;}
	const Alliance& GetAlliance() {return m_alliance;}
	SimModuleBase* GetSimModule() {return m_simModule;}
	
	/**
	 * Updates this FieldObject.
	 * @param simTimeMS the milliseconds of which to increment the state of this.
	 * @param delta_simTimeSec the seconds of which to increment the state of this.
	 */
	virtual void Update(uint32_t simTimeMS, double delta_simTimeSec);

protected:
	virtual void init() {}
	
	/**
	 * Initializes all attributes of the FieldObject at their default values.
	 */
	void init_top();
	
	/**
	 * Clears and then adds all polyhedra in m_polyhedra to m_prevpolyhedra in order.
	 */
	void init_bottom(); //cleanup
	
	/**
	 * Indicates whether this object is intersecting the supplied object.
	 *
	 * This method should define the conditions for which this object intersects another object.
	 * @param rhs the object which is being testing against this for intersection
	 * @return true if this object is intersecting rhs
	 */
	virtual bool Intersects(FieldObject& rhs); //set object's unique definition for an intersection
	
	/**
	 * Adds a polyhedron to this object.
	 * @param polyhedron the polyhedron to be added
	 * @param pos the relative position of the polyhedron
	 * @param angle the relative angle of the polyhedron
	 * @return the index of the added polyhedron
	 */
	int addPolyhedron(const ConvexPolyhedron& polyhedron, const Vector3& pos, const Vector3& angle);
	
	/**
	 * Indicates whether or not this FieldObject is static.
	 *
	 * By default, this value is true.
	 */
	bool m_static; //static by default
	
	/**
	 * Indicates the alliance of this FieldObject.
	 *
	 * By default, this value is neutral.
	 */
	Alliance m_alliance; //neutral by default

	/**
	 * The unique identifier for this FieldObject.
	 */
	UID m_uid;
	
	/**
	 * A description of this FieldObject.
	 */
	std::string m_descriptor;
	
	/**
	 * All the polyhedra that compose this FieldObject.
	 */
	std::vector<ConvexPolyhedron> m_polyhedra;
	
	/**
	 * A copy of m_polyhedra.
	 */
	std::vector<ConvexPolyhedron> m_prevpolyhedra;
	
	/**
	 * A list corresponding to the relative positions of the polyhedra.
	 */
	std::vector<Vector3> m_relativeposs; //relative position & angle of all polyhedra
	
	/**
	 * A list corresponding to the relative angles of the polyhedra.
	 */
	std::vector<Vector3> m_relativeangles;
	
	/**
	 * The position in meters of this FieldObject.
	 */
	Vector3 m_pos;
	
	/**
	 * The angle coordinates of this FieldObject.
	 */
	Vector3 m_angle;
	
	/**
	 * The velocity of this FieldObject in meters per second.
	 */
	Vector3 m_vel;
	
	/**
	 * The rotational velocity of this FieldObject in degrees per second.
	 */
	Vector3 m_rotvel;
	
	class SimModuleWrapper : public SimModuleBase {
	public:
		SimModuleWrapper(FieldObject* fieldobject)
		: m_fieldobject(fieldobject)
		{}
		virtual ~SimModuleWrapper() {}
		virtual void Update(uint32_t simTimeMS, double delta_simTimeSec) {m_fieldobject->Update(simTimeMS,delta_simTimeSec);}
		void AddModuleToSources(SimModuleBase* module) {m_sources.push_back(module);}
	private:
		FieldObject* m_fieldobject;
		
	};
	SimModuleWrapper* m_simModule;
};
#endif
