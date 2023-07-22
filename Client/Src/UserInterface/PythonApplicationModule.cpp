#if defined(ENABLE_CHEST_OPEN_RENEWAL)
	PyModule_AddIntConstant(poModule, "ENABLE_CHEST_OPEN_RENEWAL", 1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_CHEST_OPEN_RENEWAL", 0);
#endif
