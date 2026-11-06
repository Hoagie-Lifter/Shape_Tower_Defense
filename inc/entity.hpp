#pragma once

#include "components.hpp"
#include <string>
#include <tuple>

class EntityManager;

using ComponentTuple = std::tuple<
	CCollision,
	CInput,
	CLifespan,
	CScore,
	CShape,
	CTransform
>;

class Entity
{
	friend class EntityManager;
	
	bool			m_active = true;
	ComponentTuple  m_components;
	size_t			m_id = 0;
	std::string		m_tag = "default";
	
	Entity(const size_t & id, const std::string & tag)
		: m_tag(tag)
		, m_id(id)
	{}
	
public:
	bool isActive() const
	{
		return m_active;
	}
	
	void destroy()
	{
		m_active = false;
	}
	
	size_t id() const
	{
		return m_id;
	}
	
	const std::string & tag() const
	{
		return m_tag
	}
	
	
	
	
};
	