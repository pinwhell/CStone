if(NOT TARGET capstone)
	set(CAPSTONE_M68K_SUPPORT OFF)
	set(CAPSTONE_MIPS_SUPPORT OFF)
	set(CAPSTONE_PPC_SUPPORT OFF)
	set(CAPSTONE_SPARC_SUPPORT OFF)
	set(CAPSTONE_SYSZ_SUPPORT OFF)
	set(CAPSTONE_XCORE_SUPPORT OFF)
	set(CAPSTONE_X86_SUPPORT OFF)
	set(CAPSTONE_MOS65XX_SUPPORT OFF)
	set(CAPSTONE_WASM_SUPPORT OFF)
	set(CAPSTONE_BPF_SUPPORT OFF)
	set(CAPSTONE_RISCV_SUPPORT OFF)
	set(CAPSTONE_SH_SUPPORT OFF)
	set(CAPSTONE_TRICORE_SUPPORT OFF)
	set(CAPSTONE_ALPHA_SUPPORT OFF)
	set(CAPSTONE_HPPA_SUPPORT OFF)
	set(CAPSTONE_TMS320C64X_SUPPORT OFF)
	set(CAPSTONE_M680X_SUPPORT OFF)
	set(CAPSTONE_EVM_SUPPORT OFF)
	add_subdirectory(capstone)
endif()

if(NOT TARGET fmt)
	add_subdirectory(fmt)
endif()