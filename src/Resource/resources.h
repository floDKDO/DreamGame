#pragma once

#include <unordered_map>

template <typename Tid, typename Tres>
class Resources;

//"hack" pour rendre la variable "global_id_" commune à toutes les instances de la class template "Resources" mais sans pour autant la rendre globale à tout le code
struct GlobalId
{
	template <typename Tid, typename Tres>
	friend class Resources;

	public:
		static std::size_t get()
		{
			return global_id_;
		}

	private:
		inline static std::size_t global_id_ = 0ULL;
};

template <typename Tid, typename Tres>
class Resources
{
	public:
		std::size_t insert_resource(Tid resource_id, Tres&& resource)
		{
			auto p = resources_.insert(std::make_pair(resource_id, std::move(resource)));
			if(!p.second)
			{
				//récupérer la clef associée à la valeur qui a fait échouer l'insertion dans resources_
				for(auto& [key, value] : ids_) //TODO : mettre ce code dans une fonction commune
				{
					if(value == (p.first)->first) //Existe forcément
					{
						return key;
					}
				}
				return std::numeric_limits<std::size_t>::max();
			}
			else
			{
				ids_.insert({GlobalId::global_id_, resource_id});
				return GlobalId::global_id_++;
			}
		}

		void clear()
		{
			resources_.clear();
			ids_.clear();
		}

		std::unordered_map<Tid, Tres> resources_;
		std::unordered_map<std::size_t, Tid> ids_;
};

//spécialisation pour les ressources qui ont un std::size_t en clef
template <typename Tres>
class Resources<std::size_t, Tres>
{
	public:
		std::size_t insert_resource(Tres&& resource)
		{
			auto p = resources_.insert(std::make_pair(GlobalId::global_id_, std::move(resource)));
			if(!p.second)
			{
				return (p.first)->first; //retourner la clef déjà existante
			}
			else
			{
				return GlobalId::global_id_++;
			}
		}

		void clear()
		{
			resources_.clear();
		}

		std::unordered_map<std::size_t, Tres> resources_;
};