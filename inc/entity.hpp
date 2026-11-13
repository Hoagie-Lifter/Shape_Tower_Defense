#pragma once

#include "components.hpp"
#include <string>
#include <tuple>
#include <utility>

class EntityManager;

using ComponentTuple = std::tuple<
	CCircle,
	CCollision,
	CInput,
	CLifespan,
	CRectangle,
	CScore,
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
		: m_tag(tag), m_id(id) {}
	
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
		return m_tag;
	}
	
	template <typename T, typename... TArgs>
	T & add(TArgs &&... mArgs)
	{
		auto & component = std::get<T>(m_components);
		component = T(std::forward<TArgs>(mArgs)...);
		component.exists = true;
		return component;
	}
	
	template <typename T>
	T & get()
	{
		return std::get<T>(m_components);
	}

	template <typename T>
	bool has() const
	{
		return std::get<T>(m_components).exists;
	}
};
	