// Search for: void			ReceiveItem(LPCHARACTER from, LPITEM item);
// Add after:

#ifdef ENABLE_CHEST_OPEN_RENEWAL
		bool			OpenChestItem(TItemPos pos, BYTE bOpenCount = 1);
		bool			OpenChestItem(LPITEM item, BYTE bOpenCount = 1);
#endif