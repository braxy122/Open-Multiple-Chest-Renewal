// Search for ITEM_GIFTBOX: case in ::UseItemEx( and replace it with:
		case ITEM_GIFTBOX:
			{
				// ENABLE_CHEST_OPEN_RENEWAL replace
				OpenChestItem(item);
			}
			break;

//Search and add before: 
//	::GiveItemFromSpecialItemGroup(

#ifdef ENABLE_CHEST_OPEN_RENEWAL
bool CHARACTER::OpenChestItem(TItemPos pos, BYTE bOpenCount)
{
	LPITEM item;
	if (!CanHandleItem())
		return false;
	
	if (!IsValidItemPosition(pos) || !(item = GetItem(pos)))
		return false;
	
	if (item->IsExchanging())
		return false;
	
	if (IsStun())
		return false;
	
	return OpenChestItem(item, bOpenCount);
}

bool CHARACTER::OpenChestItem(LPITEM item, BYTE bOpenCount)
{
	if (!item)
		return false;
	
	if (!CanHandleItem())
		return false;
	
	DWORD dwGroupNum = item->GetVnum();
	const CSpecialItemGroup* pGroup = ITEM_MANAGER::instance().GetSpecialItemGroup(dwGroupNum);
	if (!pGroup)
	{
		sys_err("cannot find special item group %d", dwGroupNum);
		return false;
	}
	
	if (item->isLocked())
	{
		ChatPacket(CHAT_TYPE_INFO, LC_TEXT("<창고> 이 아이템은 넣을 수 없습니다."));
		return false;
	}
	
	BYTE loopCount = MINMAX(1, MIN(bOpenCount, item->GetCount()), 200);
	if (loopCount > item->GetCount())
	{
		sys_err("loopCount > item->GetCount() %d", dwGroupNum);
		return false;
	}
	
	item->Lock(true);
	if (test_server)
		ChatPacket(CHAT_TYPE_INFO, "Opening %d times of %d", loopCount, dwGroupNum);
	
	// umap of all itemVnums|Counts given at the end
	std::unordered_map<DWORD, long long> wGivenItems;
	for (auto oCount = 0; oCount < loopCount; oCount++)
	{
		std::vector<int> itemIndexes;
		int itemIndexesCount = pGroup->GetMultiIndex(itemIndexes);
		
		for (auto index = 0; index < itemIndexesCount; index++)
		{
			DWORD dwVnum = pGroup->GetVnum(itemIndexes[index]);
			DWORD dwCount = pGroup->GetCount(itemIndexes[index]);
			
			long long llItemCount = wGivenItems.count(dwVnum) > 0 ? wGivenItems[dwVnum] + dwCount : dwCount;
			wGivenItems[dwVnum] = llItemCount;
		}
	}
	
	if (wGivenItems.size() < 1)
	{
		ChatPacket(CHAT_TYPE_TALKING, LC_TEXT("아무것도 얻을 수 없었습니다."));
		return false;
	}
	
	item->SetCount(MAX(0, item->GetCount() - loopCount));
	if (item)
		item->Lock(false);
	
	for (auto& info : wGivenItems)
	{
		DWORD dwItemVnum = info.first;
		long long llItemCount = info.second;
		
		LPITEM pItem;
		switch (dwItemVnum)
		{
			case CSpecialItemGroup::GOLD:
			{
				PointChange(POINT_GOLD, llItemCount);
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("돈 %lld 냥을 획득했습니다."), llItemCount);
			}
			break;
			
			case CSpecialItemGroup::EXP:
			{
				PointChange(POINT_EXP, llItemCount);
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("상자에서 부터 신비한 빛이 나옵니다."));
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("%lld의 경험치를 획득했습니다."), llItemCount);
			}
			break;
			
			case CSpecialItemGroup::MOB:
			{
				int x = GetX() + number(-500, 500);
				int y = GetY() + number(-500, 500);
				
				LPCHARACTER ch = CHARACTER_MANAGER::instance().SpawnMob(llItemCount, GetMapIndex(), x, y, 0, true, -1);
				if (ch)
					ch->SetAggressive();
				
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("상자에서 몬스터가 나타났습니다!"));
			}
			break;
			
			case CSpecialItemGroup::SLOW:
			{
				sys_log(0, "CSpecialItemGroup::SLOW %d", -(int)llItemCount);
				AddAffect(AFFECT_SLOW, POINT_MOV_SPEED, -(int)llItemCount, AFF_SLOW, 300, 0, true);
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("상자에서 나온 빨간 연기를 들이마시자 움직이는 속도가 느려졌습니다!"));
			}
			break;
			
			case CSpecialItemGroup::DRAIN_HP:
			{
				int iDropHP = GetMaxHP()*llItemCount/100;
				iDropHP = MIN(iDropHP, GetHP()-1);
				PointChange(POINT_HP, -iDropHP);
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("상자가 갑자기 폭발하였습니다! 생명력이 감소했습니다."));
			}
			break;
			
			case CSpecialItemGroup::POISON:
			{
				AttackedByPoison(NULL);
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("상자에서 나온 녹색 연기를 들이마시자 독이 온몸으로 퍼집니다!"));
			}
			break;
			
#ifdef ENABLE_WOLFMAN_CHARACTER
			case CSpecialItemGroup::BLEEDING:
			{
				AttackedByBleeding(NULL);
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("상자에서 나온 녹색 연기를 들이마시자 독이 온몸으로 퍼집니다!"));
			}
			break;
#endif
			case CSpecialItemGroup::MOB_GROUP:
			{
				int sx = GetX() - number(300, 500);
				int sy = GetY() - number(300, 500);
				int ex = GetX() + number(300, 500);
				int ey = GetY() + number(300, 500);
				CHARACTER_MANAGER::instance().SpawnGroup(llItemCount, GetMapIndex(), sx, sy, ex, ey, NULL, true);
				ChatPacket(CHAT_TYPE_INFO, LC_TEXT("상자에서 몬스터가 나타났습니다!"));
			}
			break;
			
			default:
			{
				if (llItemCount > g_bItemCountLimit)
				{
					for(auto itemIndex = llItemCount; itemIndex > 0; itemIndex -= g_bItemCountLimit)
					{
						WORD wCount = MIN(itemIndex, g_bItemCountLimit);
						pItem = AutoGiveItem(dwItemVnum, wCount, -1, false);
					}
				}
				else if ((pItem = AutoGiveItem(dwItemVnum, llItemCount, -1, false)))
				{
					if (!pItem->IsStackable() && llItemCount > 1)
					{
						for (auto itemIndex = 0; itemIndex < (llItemCount - 1); itemIndex++)
							pItem = AutoGiveItem(dwItemVnum, 1, -1, false);
					}
				}
				
				// std::string itemName = "[IN;" + std::to_string(dwItemVnum) + "]";
				// if (llItemCount > 1)
					// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("상자에서 %s 가 %lld 개 나왔습니다."), itemName.c_str(), llItemCount);
				// else
					// ChatPacket(CHAT_TYPE_INFO, LC_TEXT("상자에서 %s 가 나왔습니다."), itemName.c_str());
				
				if (pItem)
				{
					if (llItemCount > 1)
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("상자에서 %s 가 %lld 개 나왔습니다."), pItem->GetName(), llItemCount);
					else
						ChatPacket(CHAT_TYPE_INFO, LC_TEXT("상자에서 %s 가 나왔습니다."), pItem->GetName());
				}
			}
			break;
		}
	}
	
	return true;
}
#endif
