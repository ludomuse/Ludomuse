//////////////////////////////// templates specializations 

/// \brief the specialisation building entities in a entity
template <>
inline void CJsonParser::ParseJson(RefJsonNode a_rJsonNode, CSceneNode* a_pSceneNode)
{

	assert(a_rJsonNode["object"].IsString());
	// the object type
	std::string sType = a_rJsonNode["object"].GetString();
	// the object parameters
	RefJsonNode rParams = a_rJsonNode["params"];

	CNode* pEntity;

	if (sType == "Grid")
	{

	}

	else if (sType == "Image")
	{
		pEntity = new CSpriteNode(rParams["source"].GetString(),
			EAnchor::FLOAT, rParams["x"].GetInt(), rParams["y"].GetInt());
	}


	else if (sType == "Info")
	{

	}


	else if (sType == "Input")
	{

	}


	else if (sType == "Nav")
	{

	}


	else if (sType == "Reward")
	{

	}


	else if (sType == "Sound")
	{

	}


	else if (sType == "Text")
	{
		std::string sFontName = rParams["font"].GetString();
		if (sFontName == "")
		{
			sFontName = "Default";
		}
		pEntity = new CLabelNode(rParams["content"].GetString(),
			"", // TODO: FontName
			EAnchor(rParams["anchor"].GetInt())); // TODO check if constructor with index works

	}


	else if (sType == "Video")
	{

	}


	a_pSceneNode->AddChildNode(pEntity);

}


/// \brief the specialisation building a scene
template <>
inline void CJsonParser::ParseJson(RefJsonNode a_rJsonNode, CNode* a_pNode)
{
	assert(a_rJsonNode.HasMember("entities"));
	CSceneNode* pSceneNode = new CSceneNode();
	a_pNode->AddChildNode(pSceneNode);
	RefJsonNode rEntities = a_rJsonNode["entities"];
	for (int i = 0; i < rEntities.Size(); ++i)
	{
		ParseJson(rEntities[i], pSceneNode);
	}
}


template <typename T>
void CJsonParser::ParseJson(RefJsonNode a_rJsonNode, T* a_pNode)
{
	// not supposed to be called

	//assert(a_rJsonNode.HasMember("screens"));
	//RefJsonNode rScenes = a_rJsonNode["screens"];
	//for (int i = 0; i < rScenes.Size(); ++i)
	//{
	//	ParseJson(rScenes[i], (CSceneNode*) a_pNode);
	//}
}

