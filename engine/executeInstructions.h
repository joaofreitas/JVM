/*
 * executeInstructions.h
 *
 *  Created on: 25/01/2011
 *      Author: joaofreitas
 */

#ifndef EXECUTEINSTRUCTIONS_H_
#define EXECUTEINSTRUCTIONS_H_

#include <inttypes.h>
#include "../structures/classFileStructures.h"

typedef struct npair_struct
{
	int32_t match;
	int32_t offset;
}pair_item;

void func_execInstruction();

double getDouble(u4 low_bytes, u4 high_bytes);
u4 getDoubleHighBytes(double value);
u4 getDoubleLowBytes(double value);

u8 getLong(u4 low_bytes, u4 high_bytes);
u4 getLongHighBytes(u8 value);
u4 getLongLowBytes(u8 value);


void func_aaload();
void func_aastore();/*boi*/
void func_aconst_null();
void func_aload();
void func_aload_0();
void func_aload_1();/*boi*/
void func_aload_2();
void func_aload_3();
void func_anewarray();
void func_areturn();/*boi*/
void func_arraylength();
void func_astore();
void func_astore_0();
void func_astore_1();/*boi*/
void func_astore_2();
void func_astore_3();
void func_athrow();

void func_baload();/*boi*/
void func_bastore();
void func_bipush();

void func_caload();
void func_castore();/*boi*/
void func_checkcast();

void func_d2f();
void func_d2i();
void func_d2l();/*boi*/
void func_dadd();
void func_daload();
void func_dastore();
void func_dcmpg();/*boi*/
void func_dcmpl();
void func_dconst_0();
void func_dconst_1();
void func_ddiv();/*boi*/
void func_dload();
void func_dload_0();
void func_dload_1();
void func_dload_2();/*boi*/
void func_dload_3();
void func_dmul();
void func_dneg();
void func_drem();/*boi*/
void func_dreturn();
void func_dstore();
void func_dstore_0();
void func_dstore_1();/*boi*/
void func_dstore_2();
void func_dstore_3();
void func_dsub();
void func_dup();/*boi*/
void func_dup_x1();
void func_dup_x2();
void func_dup2();
void func_dup2_x1();/*boi*/
void func_dup2_x2();

void func_f2d();
void func_f2i();
void func_f2l();/*boi*/
void func_fadd();
void func_faload();
void func_fastore();
void func_fcmpg();/*boi*/
void func_fcmpl();
void func_fconst_0();
void func_fconst_1();
void func_fconst_2();/*boi*/
void func_fdiv();
void func_fload();
void func_fload_0();
void func_fload_1();/*boi*/
void func_fload_2();
void func_fload_3();
void func_fmul();
void func_fneg();/*boi*/
void func_frem();
void func_freturn();
void func_fstore();
void func_fstore_0();/*boi*/
void func_fstore_1();
void func_fstore_2();
void func_fstore_3();
void func_fsub();/*boi*/

void func_getfield();
void func_getstatic();
void func_goto();
void func_goto_w();/*boi*/

void func_i2b();
void func_i2c();
void func_i2d();
void func_i2f();/*boi*/
void func_i2l();
void func_i2s();
void func_iadd();
void func_iaload();/*boi*/
void func_iand();
void func_iastore();
void func_iconst_m1();
void func_iconst_0();/*boi*/
void func_iconst_1();
void func_iconst_2();
void func_iconst_3();
void func_iconst_4();/*boi*/
void func_iconst_5();
void func_idiv();
void func_if_acmpeq();
void func_if_acmpne();/*boi*/
void func_if_icmpeq();
void func_if_icmpne();
void func_if_icmplt();
void func_if_icmpge();/*boi*/
void func_if_icmpgt();
void func_if_icmple();
void func_ifeq();
void func_ifne();/*boi*/
void func_iflt();
void func_ifge();
void func_ifgt();
void func_ifle();/*boi*/
void func_ifnonnull();
void func_ifnull();
void func_iinc();
void func_iload();/*boi*/
void func_iload_0();
void func_iload_1();
void func_iload_2();
void func_iload_3();/*boi*/
void func_imul();
void func_ineg();
void func_instanceof();
void func_invokeinterface();/*boi*/
void func_invokespecial();
void func_invokestatic();
void func_invokevirtual();
void func_ior();/*boi*/
void func_irem();
void func_ireturn();
void func_ishl();
void func_ishr();/*boi*/
void func_istore();
void func_istore_0();
void func_istore_1();
void func_istore_2();/*boi*/
void func_istore_3();
void func_isub();
void func_iushr();
void func_ixor();/*boi*/

void func_jsr();
void func_jsr_w();

void func_l2d();
void func_l2f();/*boi*/
void func_l2i();
void func_ladd();
void func_laload();
void func_land();/*boi*/
void func_lastore();
void func_lcmp();
void func_lconst_0();
void func_lconst_1();/*boi*/
void func_ldc();
void func_ldc_w();
void func_ldc2_w();
void func_ldiv();/*boi*/
void func_lload();
void func_lload_0();
void func_lload_1();
void func_lload_2();/*boi*/
void func_lload_3();
void func_lmul();
void func_lneg();
void func_lookupswitch();/*boi*/
void func_lor();
void func_lrem();
void func_lreturn();
void func_lshl();/*boi*/
void func_lshr();
void func_lstore();
void func_lstore_0();
void func_lstore_1();/*boi*/
void func_lstore_2();
void func_lstore_3();
void func_lsub();
void func_lushr();/*boi*/
void func_lxor();

void func_monitorenter();
void func_monitorexit();
void func_multianewarray();/*boi*/

void func_new();
void func_newarray();
void func_nop();

void func_pop();/*boi*/
void func_pop2();
void func_putfield();
void func_putstatic();

void func_ret();/*boi*/
void func_return();

void func_saload();
void func_sastore();
void func_sipush();/*boi*/
void func_swap();

void func_tableswitch();

void func_wide();

void func_breakpoint();/*boi*/
void func_impdep1();
void func_impdep2();
#endif /* EXECUTEINSTRUCTIONS_H_ */
