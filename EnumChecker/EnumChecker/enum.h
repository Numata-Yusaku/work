#ifndef __ENUM_H__
#define __ENUM_H__
enum EN_CHAR_CLASSID
{
	E_CHAR_CLASSID_A = 0x01,
	E_CHAR_CLASSID_B,
	E_CHAR_CLASSID_C,
	E_CHAR_CLASSID_D,
	E_CHAR_CLASSID_E,

	E_CHAR_CLASSID_FF = 0xFF,
};
enum EN_CHAR_KIND
{
	/**/
	E_CHAR_KIND_UNDEFINE = (E_CHAR_CLASSID_B << 24) | 0x00000001,
	E_CHAR_KIND_STRING,
	E_CHAR_KIND_NUMBER,
	E_CHAR_KIND_OPERATOR_SINGLE,
	E_CHAR_KIND_OPERATOR_MULTI,
	E_CHAR_KIND_PERIOD,
	E_CHAR_KIND_SKIP,

	E_CHAR_KIND_INVALID		=0xFFFFFFFF,
};
#endif /* __ENUM_H__ */
