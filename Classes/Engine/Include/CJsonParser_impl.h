//////////////////////////////// templates specializations 

/// \brief the specialisation building entities in a entity
template <>
void CJsonParser::ParseJson(RefJsonNode a_rJsonNode, CSceneNode* a_pNode)
{
	assert(a_rJsonNode.HasMember("entities"));
	RefJsonNode rEntities = a_rJsonNode["entities"];
	for (int i = 0; i < rEntities.Size(); ++i)
	{
		assert(rEntities[i]["object"].IsString());
		// the object type
		std::string sType = rEntities[i]["object"].GetString();
		// the object parameters
		RefJsonNode rParams = rEntities[i]["params"];

		CNode* pNode;

		if (sType == "Grid")
		{

		}

		else if (sType == "Image")
		{
			pNode = new CSpriteNode(rParams["source"].GetString(),
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
			pNode = new CLabelNode(rParams["content"].GetString(),
				"", // TODO: FontName
				EAnchor(rParams["anchor"].GetInt())); // TODO check if constructor with index works

		}


		else if (sType == "Video")
		{

		}


		a_pNode->AddChildNode(pNode);
	}
}


/// \brief the specialisation building a scene
template <>
void CJsonParser::ParseJson(RefJsonNode a_rJsonNode, CNode* a_pNode)
{
	assert(a_rJsonNode.HasMember("content"));
	CSceneNode* pSceneNode = new CSceneNode();
	a_pNode->AddChildNode(pSceneNode);
	ParseJson(a_rJsonNode["content"], pSceneNode);
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

