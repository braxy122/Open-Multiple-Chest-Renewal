// Search:
HEADER_CG_ITEM_DROP2

// Add after
#ifdef ENABLE_CHEST_OPEN_RENEWAL
	HEADER_CG_CHEST_OPEN			= 22,
#endif

// Search:
typedef struct command_item_use

// Add after
#ifdef ENABLE_CHEST_OPEN_RENEWAL
typedef struct OpenChestPacket
{
	BYTE header;
	TItemPos pos;
	WORD wOpenCount;
} TPacketCGOpenChest;
#endif