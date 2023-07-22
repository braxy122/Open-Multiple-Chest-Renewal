# Search for:
	def __UseItem(self, slotIndex):
		ItemVNum = player.GetItemIndex(slotIndex)
		item.SelectItem(ItemVNum)

# Add after:
		if (item.GetItemType() == item.ITEM_TYPE_GIFTBOX) and app.IsPressed(app.DIK_LCONTROL):
			itemUseCount = player.GetItemCount(slotIndex) if player.GetItemCount(slotIndex) <= 200 else 200
			net.SendOpenChestPacket(slotIndex, itemUseCount)
			return