// Search:
void CInputMain::ItemMove(LPCHARACTER ch, const char * data)

// Add after
#ifdef ENABLE_CHEST_OPEN_RENEWAL
void CInputMain::ChestOpen(LPCHARACTER ch, const char * data)
{
	struct OpenChestPacket * pinfo = (struct OpenChestPacket *) data;
	if (ch)
		ch->OpenChestItem(pinfo->pos, static_cast<WORD>(pinfo->wOpenCount));
}
#endif

// Search:
		case HEADER_CG_ITEM_DROP2:

// Add after
#ifdef ENABLE_CHEST_OPEN_RENEWAL
		case HEADER_CG_CHEST_OPEN:
			if (!ch->IsObserverMode())
				ChestOpen(ch, c_pData);
			break;
#endif
