# Note: if you don't have ENABLE_EMOJI_SYSTEM, just skip this part.

# At the end of the def AddItemData( method, before:
	self.ShowToolTip()
	
# Add:
	if app.ENABLE_EMOJI_SYSTEM and (itemType == item.ITEM_TYPE_GIFTBOX) and window_type in (player.INVENTORY,):
		itemUseCount = player.GetItemCount(window_type, slotIndex) if player.GetItemCount(window_type, slotIndex) <= 200 else 200
		if itemUseCount > 1:
			self.AppendSpace(5)
			self.AppendTextLine("{} + {} {}".format(Emoji("icon\emoji\key_ctrl.png"),  Emoji("icon\emoji\key_rclick.png"), "Open {}x items".format(itemUseCount)))