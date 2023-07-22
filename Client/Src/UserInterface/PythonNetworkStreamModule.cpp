// Search for:
PyObject* netSendItemUseToItemPacket(PyObject* poSelf, PyObject* poArgs)

// Add before:
#ifdef ENABLE_CHEST_OPEN_RENEWAL
PyObject* netSendOpenChestPacket(PyObject* poSelf, PyObject* poArgs)
{
	TItemPos Cell;
	if (!PyTuple_GetInteger(poArgs, 0, &Cell.cell))
		return Py_BuildException();

	int iOpenCount;
	if (!PyTuple_GetInteger(poArgs, 1, &iOpenCount))
		return Py_BuildException();

	CPythonNetworkStream& rkNetStream=CPythonNetworkStream::Instance();
	rkNetStream.SendOpenChestPacket(Cell, iOpenCount);
	return Py_BuildNone();
}
#endif

// Search for:
		{ "SendItemUseToItemPacket",			netSendItemUseToItemPacket,				METH_VARARGS },

// Add after:
#ifdef ENABLE_CHEST_OPEN_RENEWAL
		{ "SendOpenChestPacket",				netSendOpenChestPacket,					METH_VARARGS },
#endif