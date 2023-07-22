// Search for:
		bool SendItemUsePacket(TItemPos pos);
// Add after:
#ifdef ENABLE_CHEST_OPEN_RENEWAL
		bool SendOpenChestPacket(TItemPos pos, WORD wOpenCount);
#endif