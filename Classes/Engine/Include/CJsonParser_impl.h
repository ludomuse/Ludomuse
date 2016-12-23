//////////////////////////////// templates specializations 


/// \brief the specialisation building entities in a scene or subentities
template <typename T>
inline void CJsonParser::ParseJson(RefJsonNode a_rJsonNode, T* a_pNode, bool a_bNodeVisible)
{

	assert(a_rJsonNode["type"].IsString());
	// the object type
	std::string sType = a_rJsonNode["type"].GetString();
	// the object parameters
	RefJsonNode rParams = a_rJsonNode["params"];

	int x = 0, y = 0, width = 0, height = 0;
	std::string id;
	if (rParams.HasMember("x") && rParams.HasMember("y")) 
	{
		x = rParams["x"].GetInt();
		y = rParams["y"].GetInt();
	}
	if (rParams.HasMember("width")) 
	{
		width = rParams["width"].GetInt();
	}
	if (rParams.HasMember("height"))
	{
		height = rParams["height"].GetInt();
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
            sBackgroundSource = m_sBasePath + sBackgroundSource;
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
	/*else if (sType == "Dummy")
	{
		CCLOG("Grid construction");
           pEntity = new CMenuNode(
               rParams["normal"].GetString(),
               rParams["selected"].GetString(),
               CCallback<CKernel, cocos2d::Ref*>(m_pKernel,
				   std::string(rParams["action"].GetString()) == "connect" ?
                         &CKernel::Connect : &CKernel::SendMessage),
               IntToAnchor(rParams["anchor"].GetInt()),
               width,
               height,
               x,
               y);
		   CCLOG("grid constructed");
    }*/

	else if (sType == "Image" || sType == "Info")
	{
        std::string imageSource = m_sBasePath + rParams["source"].GetString();
        pEntity = new CSpriteNode(imageSource,
			IntToAnchor(rParams["anchor"].GetInt()),
			width, height,
			x, y);
	}

	else if (sType == "Scratch")
	  {
	    std::string imageSource = m_sBasePath + rParams["source"].GetString();
	    pEntity = new CScratchNode(imageSource,
				       IntToAnchor(rParams["anchor"].GetInt()),
				       width, height, x, y);
	  }

	else if (sType == "Video")
	{
		pEntity = new CVideoNode(m_sBasePath + rParams["source"].GetString(),
			IntToAnchor(rParams["anchor"].GetInt()),
				width, height, x, y);
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
            m_sBasePath + rParams["normal"].GetString(),
            m_sBasePath + rParams["selected"].GetString(),
			CCallback<CKernel, cocos2d::Ref*>(m_pKernel,
				(std::string(rParams["action"].GetString()) == "next") ?
				&CKernel::NavNext : &CKernel::NavPrevious),
			IntToAnchor(rParams["anchor"].GetInt()),
			width,
			height,
			x,
			y);

	}

	else if (sType == "Validator")
	{
		CValidator* oValidator = new CValidator(m_pKernel);
		for (int i = 0; i < rParams["ids"].Size(); ++i)
		{
			oValidator->AddID(rParams["ids"][i].GetString());
		}

		if (rParams.HasMember("sound"))
            oValidator->SetSound( rParams["sound"].GetString());

		a_pNode->AddChildNode(oValidator);
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
		if (rParams.HasMember("source"))
		{
            sFontName =  m_sBasePath + rParams["source"].GetString();
		}
		if (sFontName == "") // apply default font
		{
            sFontName =  m_sBasePath + "fonts/Open_Sans/OpenSans-Regular.ttf";
		}

		int iFontSize = 24;
		if (rParams.HasMember("fontSize"))
		{
            iFontSize =  rParams["fontSize"].GetInt();
		}
		if (iFontSize < 1)
		{
			iFontSize = 24;
		}
		std::string sTextAlign = "center";
		if (rParams.HasMember("textAlign"))
		{
			sTextAlign = rParams["textAlign"].GetString();
		}

		std::string sColor = "";
		if (rParams.HasMember("color"))
		{
			sColor = rParams["color"].GetString();
		}
		pEntity = new CLabelNode(rParams["content"].GetString(),
			sFontName,
			iFontSize,
			sTextAlign,
			sColor,
			IntToAnchor(rParams["anchor"].GetInt()),
			width,
			height,
			x,
			y);

	}


	else if (sType == "EditBox")
	{
        pEntity = new CSpriteNode( m_sBasePath + "ui/textfieldBackground.png", IntToAnchor(rParams["anchor"].GetInt()), width, height, x, y);
		CEditBoxNode* pEditBox = new CEditBoxNode(EAnchor::CENTER, 100);
		pEntity->AddChildNode(pEditBox);
	}

	else if (sType == "Video")
	{

	}


	else if (sType == "Peers")
	{
		pEntity = new CPeerNode(m_pKernel,
			rParams["children"][0],
			IntToAnchor(rParams["anchor"].GetInt()),
			width,
			height,
			x,
			y);
	}

	else if (sType == "Camera")
	{
		if (rParams.HasMember("isReceiver")) 
		{
			pEntity = new CCameraFeedNode(
                 m_sBasePath + rParams["source"].GetString(),
				IntToAnchor(rParams["anchor"].GetInt()),
				width,
				height,
				x,
				y,
				rParams["isReceiver"].GetBool());
		}
		else
		{
			pEntity = new CCameraFeedNode(
                 m_sBasePath + rParams["source"].GetString(),
				IntToAnchor(rParams["anchor"].GetInt()),
				width,
				height,
				x,
				y);
		}
		
	}


	// check listeners
	if (rParams.HasMember("listeners"))
	{
		RefJsonNode rListeners = rParams["listeners"];
		for (int i = 0; i < rListeners.Size(); ++i)
		{
			a_bNodeVisible = ParseCallback(rListeners[i], pEntity) && a_bNodeVisible;
		}
	}

	if (pEntity)
		pEntity->SetVisible(a_bNodeVisible);

	// recursively parse children
	if (rParams.HasMember("children"))
	{
		RefJsonNode rEntities = rParams["children"];
		for (int i = 0; i < rEntities.Size(); ++i)
		{
			ParseJson(rEntities[i], pEntity, a_bNodeVisible);
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
			ParseJson(rActiveEntities[i], pActive, a_bNodeVisible);
		}
		RefJsonNode rInactiveEntities = rParams["children-inactive"];
		for (int i = 0; i < rInactiveEntities.Size(); ++i)
		{
			ParseJson(rInactiveEntities[i], pInactive, a_bNodeVisible);
		}

		pEntity->AddChildNode(pSequence);
	}

	if (pEntity != nullptr) {
		if (a_rJsonNode.HasMember("id"))
		{
			pEntity->SetID(a_rJsonNode["id"].GetString());
		}
		a_pNode->AddChildNode(pEntity);
	}

}


/// \brief the specialisation building a scene
template <>
inline void CJsonParser::ParseJson(RefJsonNode a_rJsonNode, CNode* a_pNode, bool a_bNodeVisible)
{
	CSceneNode* pSceneNode = new CSceneNode(a_rJsonNode["scene"].GetString(), m_pKernel);
	if (a_rJsonNode["scene"].GetString() == std::string("Dashboard"))
    {
		m_pKernel->m_pDashboard = pSceneNode;
    }
    else if (a_rJsonNode["scene"].GetString() == std::string("WaitingScene"))
    {
        m_pKernel->m_pWaitingScene = pSceneNode;
    }
    else
    {
		a_pNode->AddChildNode(pSceneNode);
    }
    
	if (a_rJsonNode.HasMember("sync"))
	{
		pSceneNode->SetSynced(a_rJsonNode["sync"].GetBool());
	}
	
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
	if (a_rJsonNode.HasMember("synced"))
	{
		pSceneNode->SetSynced(a_rJsonNode["synced"].GetBool());
	}
	if (a_rJsonNode.HasMember("dashboardTrigger"))
	{
		pSceneNode->m_bDashboardTrigger = a_rJsonNode["dashboardTrigger"].GetBool();
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

