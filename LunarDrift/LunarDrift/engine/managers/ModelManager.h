#pragma once

#include "Manager.h"
#include "../containers/Model.h"
#include <map>

/**
 * @brief A manager class for handling and passing around model data
 *
 * @author Hayley Hatton
 * @date   13/03/2016
 * @see    Manager
 * @ingroup Managers
 */
class ModelManager : public Manager
{
public:
    /** @brief Default constructor */
	ModelManager();

    /** @brief Default destructor */
	~ModelManager();


    /**
     * @brief Load a model from an OBJ file at a given path
     * @param context Graphics context
     * @param id Identifying name of the model
     * @param path Path to the OBJ file
     * @return Weak-managed pointer to the model data class
     * @see ModelLoader
     */
	std::weak_ptr<Model> loadModel(
		Context* context,
		const std::string& id,
		const std::string& path
	);
	
    /**
     * @brief Add a model to the manager
     * @param id Identifying name of the model
     * @param model Smart pointer to the model data class
     */
	void addModel(const std::string& id, std::shared_ptr<Model> model);

    /**
     * @brief Access a model from inside the manager
     * @param id Identifying name of the model
     * @return Weak-managed pointer to the model data class; empty if not found
     */
	std::weak_ptr<Model> getModel(const std::string& id);

    /**
     * @brief Remove a model from the manager (and kill if applicable)
     * @param id Identifying name of the model
     */
	void removeModel(const std::string& id);

    /**
     * @brief Remove all models from the manager (and kill if applicable)
     */
	void removeAllModels();


private:
	std::map<std::string, std::shared_ptr<Model>> m_Models;
};
