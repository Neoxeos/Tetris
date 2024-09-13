#include "EntityManager.h"

EntityManager::EntityManager()
{

}

void EntityManager::update()
{
	// add entities from m_entitiesToAdd to the proper location
	// add them to the vector of all entities
	// add them to the vector inside the map, with the tag as a key
	for (auto& entity : m_entitiesToAdd)
	{
		m_entities.push_back(entity);
		m_entityMap[entity->m_tag].push_back(entity);
	}
	m_entitiesToAdd.clear();

	// remove dead entities from the vector of all entities 
	removeDeadEntities(m_entities);

	//remove dead entities from each vector in the entity map
	// C++20 way of iterating through [key, vlaue] pairs in a map
	for (auto& [tag, entityVec] : m_entityMap)
	{
		removeDeadEntities(entityVec);
	}
}

void EntityManager::removeDeadEntities(EntityVec& vec)
{
	// this is called by the update function
	EntityVec vecCopy;
	for (auto& entity : vec)
	{
		if (entity->isActive())
		{
			vecCopy.push_back(entity);
		}
	}
	vec = vecCopy;
}

std::shared_ptr<Entity> EntityManager::addEntity(const std::string& tag)
{
	auto entity = std::shared_ptr<Entity>(new Entity(m_totalEntities++, tag));

	m_entitiesToAdd.push_back(entity);

	return entity;
}

const EntityVec& EntityManager::getEntities()
{
	return m_entities;
}

const EntityVec& EntityManager::getEntities(const std::string& tag)
{
	return m_entityMap[tag];
}

const std::map<std::string, EntityVec>& EntityManager::getEntityMap()
{
	return m_entityMap;
}