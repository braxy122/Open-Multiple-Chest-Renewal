// Search for:
bool CPythonNetworkStream::SendItemUseToItemPacket(TItemPos source_pos, TItemPos target_pos)

// Add before:
#ifdef ENABLE_CHEST_OPEN_RENEWAL
bool CPythonNetworkStream::SendOpenChestPacket(TItemPos pos, WORD wOpenCount)
{
	if (!__CanActMainInstance())
		return true;
	__PlayInventoryItemUseSound(pos);

	TPacketCGOpenChest chestUsePacket;
	chestUsePacket.header = HEADER_CG_CHEST_OPEN;
	chestUsePacket.pos = pos;
	chestUsePacket.wOpenCount = wOpenCount;

	if (!Send(sizeof(TPacketCGOpenChest), &chestUsePacket))
	{
		Tracen("SendOpenChestPacket Error");
		return false;
	}

	return SendSequence();
}
#endif
