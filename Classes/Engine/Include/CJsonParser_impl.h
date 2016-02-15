//////////////////////////////// templates specializations 


/// \brief the specialisation building entities in a scene or subentities
template <typename T>
inline void CJsonParser::ParseJson(RefJsonNode a_rJsonNode, T* a_pNode)
{

	assert(a_rJsonNode["type"].IsString());
	// the object type
	std::string sType = a_rJsonNode["type"].GetString();
	// the object parameters
	RefJsonNode rParams = a_rJsonNode["params"];

	int x, y, width, height;
	if (rParams.HasMember("x")) 
	{
		x = rParams["x"].GetInt();
		y = rParams["y"].GetInt();
	}
	else
	{
		x = y = 0;
	}
	if (rParams.HasMember("width")) 
	{
		width = rParams["width"].GetInt();
		height = rParams["height"].GetInt();
	}
	else
	{
		width = height = 0;
	}

	CEntityNode* pEntity(nullptr);

	if (sType == "Grid")
	{
		pEntity = new CGridNode(
			rParams["rows"].GetInt(),
			rParams["cols"].GetInt(),
			IntToAnchor(rParams["anchor"].GetInt()),
			width,
			height,
			x,
			y);
	}

	else if (sType == "Group")
	{
		std::string sBackgroundSource = "";
		if (rParams.HasMember("source"))
		{
			sBackgroundSource = rParams["source"].GetString();
		}
		pEntity = new CGroupNode(
			IntToAnchor(rParams["anchor"].GetInt()),
			width,
			height,
			x,
			y,
			sBackgroundSource);
	}

	// dummy element to test Wifi Direct
	else if (sType == "Dummy")
	{
		CCLOG("Grid construction");
           pEntity = new CMenuNode(
               rParams["normal"].GetString(),
               rParams["selected"].GetString(),
               CCallback<CKernel>(m_pKernel,
				   std::string(rParams["action"].GetString()) == "connect" ?
                         &CKernel::Connect : &CKernel::SendMessage),
               IntToAnchor(rParams["anchor"].GetInt()),
               width,
               height,
               x,
               y);
		   CCLOG("grid constructed");
    }

	else if (sType == "Image" || sType == "Info")
	{
		pEntity = new CSpriteNode(rParams["source"].GetString(),
			IntToAnchor(rParams["anchor"].GetInt()),
			width, height,
			x, y);
	}


	else if (sType == "Input")
	{

	}


	else if (sType == "Nav")
	{
		// TODO
		//auto lambda = [this](cocos2d::Ref* pSender)
		//{
		//	m_pKernel->NavNext(pSender);
		//	//CCLOG("HI");
		//};
		pEntity = new CMenuNode(
			rParams["normal"].GetString(),
			rParams["selected"].GetString(),
			CCallback<CKernel>(m_pKernel,
				(std::string(rParams["action"].GetString()) == "next") ?
				&CKernel::NavNext : &CKernel::NavPrevious),
			IntToAnchor(rParams["anchor"].GetInt()),
			width,
			height,
			x,
			y);

	}


	else if (sType == "Reward")
	{

	}


	else if (sType == "Sound")
	{

	}


	else if (sType == "Text")
	{
		std::string sFontName = "";
		if (rParams.HasMember("font"))
		{
			sFontName = rParams["font"].GetString();
		}
		if (sFontName == "") // apply default font
		{
			sFontName = "fonts/Open_Sans/OpenSans-Regular.ttf";
		}
		int iFontSize = rParams["fontSize"].GetInt();
		if (iFontSize < 1)
		{
			iFontSize = 24;
		}
		std::string sTextAlign = "center";
		if (rParams.HasMember("textAlign"))
		{
			sTextAlign = rParams["textAlign"].GetString();
		}

		pEntity = new CLabelNode(rParams["content"].GetString(),
			sFontName,
			iFontSize,
			sTextAlign,
			rParams["color"].GetString(),
			IntToAnchor(rParams["anchor"].GetInt()),
			width,
			height,
			x,
			y);

	}


	else if (sType == "Video")
	{

	}


	// check listeners
	if (rParams.HasMember("listeners"))
	{
		RefJsonNode rListeners = rParams["listeners"];
		for (int i = 0; i < rListeners.Size(); ++i)
		{

		}
	}



	// recursively parse children
	if (rParams.HasMember("children"))
	{
		RefJsonNode rEntities = rParams["children"];
		for (int i = 0; i < rEntities.Size(); ++i)
		{
			ParseJson(rEntities[i], pEntity);
		}
	}
	else if (rParams.HasMember("children-active") && rParams.HasMember("children-inactive"))
	{
		CSequenceNode* pSequence = new CSequenceNode();
		CGroupNode* pActive = new CGroupNode();
		CGroupNode* pInactive = new CGroupNode();
		pSequence->AddChildNode(pInactive);
		pSequence->AddChildNode(pActive);

		RefJsonNode rActiveEntities = rParams["children-active"];
		for (int i = 0; i < rActiveEntities.Size(); ++i)
		{
			ParseJson(rActiveEntities[i], pActive);
		}
		RefJsonNode rInactiveEntities = rParams["children-inactive"];
		for (int i = 0; i < rInactiveEntities.Size(); ++i)
		{
			ParseJson(rInactiveEntities[i], pInactive);
		}

		pEntity->AddChildNode(pSequence);
	}

	if (pEntity != nullptr) {
		a_pNode->AddChildNode(pEntity);
	}

}


/// \brief the specialisation building a scene
template <>
inline void CJsonParser::ParseJson(RefJsonNode a_rJsonNode, CNode* a_pNode)
{
	CSceneNode* pSceneNode = new CSceneNode(a_rJsonNode["scene"].GetString());
	a_pNode->AddChildNode(pSceneNode);
	if (a_rJsonNode.HasMember("content"))
	{
		RefJsonNode rEntities = a_rJsonNode["content"];
		for (int i = 0; i < rEntities.Size(); ++i)
		{
			ParseJson(rEntities[i], pSceneNode);
		}
	}
	if (a_rJsonNode.HasMember("information"))
	{
		RefJsonNode rInfoItems = a_rJsonNode["information"];
		for (int i = 0; i < rInfoItems.Size(); ++i)
		{
			ParseJson(rInfoItems[i], pSceneNode);
		}
	}
	if (a_rJsonNode.HasMember("navigation"))
	{
		RefJsonNode rNavItems = a_rJsonNode["navigation"];
		for (int i = 0; i < rNavItems.Size(); ++i)
		{
			ParseJson(rNavItems[i], pSceneNode);
		}
	}
}


//template <typename T>
//void CJsonParser::ParseJson(RefJsonNode a_rJsonNode, T* a_pNode)
//{
//	// not supposed to be called
//
//	//assert(a_rJsonNode.HasMember("screens"));
//	//RefJsonNode rScenes = a_rJsonNode["screens"];
//	//for (int i = 0; i < rScenes.Size(); ++i)
//	//{
//	//	ParseJson(rScenes[i], dynamic_cast<CSceneNode*>(a_pNode));
//	//}
//}

