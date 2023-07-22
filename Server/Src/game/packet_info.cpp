// In CPacketInfoCG::CPacketInfoCG(), at the end add:
#ifdef ENABLE_CHEST_OPEN_RENEWAL
	Set(HEADER_CG_CHEST_OPEN, sizeof(TPacketCGOpenChest), "chest_open", false);
#endif