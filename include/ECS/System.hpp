#pragma once

/**
 * @file System.h
 * 
 * This file contains a small System base class. Although
 * small, this base class is incredibly important
 * to the ECS. It keeps track of a set of Entities
 * that match its Signature, and its subclasses can
 * provide whatever functionality they want.
 * 
 * @note This, and most base ECS classes, were adapted from
 * [Austin Morlan's amazing ECS writeup here.](https://austinmorlan.com/posts/entity_component_system/)
 * 
 * @author Tim Bishop
*/

#include <set>
#include "Component.hpp"
#include "Entity.hpp"

/**
 * @class System
 * 
 * The small but mighty System base class. Any systems the
 * user would want should be subclassed from this class.
*/
class System
{
public:
	/** A set of Entities that are affected by the System */
	std::set<Entity> mEntities;

	/**
	 * A virtual function returning the signature of
	 * the System. This will affect which Entities are
	 * able to be affected by the System, as well as
	 * ensuring specific Components being on each
	 * Entity.
	 * 
	 * @returns The Signature bitset of the System.
	*/
	virtual Signature GetSignature() = 0;
};

