/*
 * executeInstructions.c
 *
 *  Created on: 25/01/2011
 *      Author: joaofreitas
 */

#include "executeInstructions.h"
#include "../structures/frame.h"
#include "../structures/methodArea.h"
#include "executeEngine.h"

int wide=0;

u2 getParameterCount(char *method_descriptor) {
	u2 count;

	method_descriptor++;
	while (*method_descriptor != ')') {
		if (*method_descriptor == '[') {
			method_descriptor++;
		}
		if (*method_descriptor == 'L') {
			while (*method_descriptor != ';') {
				method_descriptor++;
			}
		}
		/*Double e long precisam ter 2 espaços*/
		if ((*method_descriptor == 'J') || (*method_descriptor == 'D')) {
			count++;
		}
		count++;
		method_descriptor++;
	}
	return count;
}


void func_aaload(){

}

void func_aastore(){
	u4 index, value;
    arrays_t  *arrey;

	value = popOperand();
	index = popOperand();
	array = (arrays_t*)popOperand();

	if (array == NULL) {
		printf("\n\nNull Pointer Exception (op_aastore)\n");
		return;
	}
	if (index >= array->size)
	{
	    printf("\n\nNull Pointer Exception (op_aastore)\n");
		return;
	}

	arrey_ref->reference[index] = value;
}

void func_aconst_null(){
}

void func_aload(){

}

void func_aload_0(){
	pushOperand(frame_stack->frame->local_variables[0]);
}

void func_aload_1(){
	pushOperand(frame_stack->frame->local_variables[1]);
}

void func_aload_2(){
	pushOperand(frame_stack->frame->local_variables[2]);
}

void func_aload_3(){
	pushOperand(frame_stack->frame->local_variables[3]);
}

void func_anewarray(){
	unsigned char indexbyte1;
	unsigned char indexbyte2;
    arrays_t *array;
	u2 index;
	u4 count;
	cp_info type;

	frame_stack->frame->pc++;
	indexbyte1 = (unsigned char)frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
	frame_stack->frame->pc++;
	indexbyte2 = (unsigned char)frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
	index = indexbyte1 << 8 | indexbyte2;

    type = getConstantPoolElementByIndex(frame_stack->frame->cp,index);

	array = calloc(1, sizeof(arrays_t));
	count = (u4)popOperand();
	array->size = count;
	array->reference = calloc(count, sizeof(void *));

	push(array);
}

void func_areturn(){
}

void func_arraylength(){
}

void func_astore(){
}

void func_astore_0(){
    frame_stack->frame->local_variables[0] = pushOperand();
}

void func_astore_1(){
}

void func_astore_2(){
}

void func_astore_3(){
}

void func_athrow(){
	// faz nada
	popOperand();
	pushOperand(0);
}


void func_baload(){
}

void func_bastore(){
}

void func_bipush(){
}


void func_caload(){
	arrays_t *arrey_ref;
	u4 index;
    u1 *array;

	index = popOperand();
	arrey_ref = (arrays_t *)popOperand();

	if (array == NULL) {
		printf("\n\nNull Pointer Exception (op_aastore)\n");
		return;
	}
	if (index >= array->size)
	{
	    printf("\n\nNull Pointer Exception (op_aastore)\n");
		return;
	}

	array = (u1 *)arrey_ref->reference;

	pushOperand(array[index]);

}

void func_castore(){
}

void func_checkcast(){
}


void func_d2f(){
}

void func_d2i(){
	u8 aux1;
	double aux2=0;
	u4 aux3, aux4;

	aux1 = (u8)popOperand();
	aux4 = popOperand();
	aux1 = aux1 << 32;
	aux1 |= aux4;
	memcpy(&aux2, &aux1, sizeof(double));


	aux3 = (u4)(aux2);
	push(aux3);
	frame_stack->frame->pc++;
}

void func_d2l(){
}

void func_dadd(){
}

void func_daload(){
}

void func_dastore(){
	u4 index, hi, low;
    arrays_t  *array;
    u1 *c_array;
    double d;

	hi = popOperand();
	low = popOperand();
	index = popOperand();
	array = (arrays_t *)popOperand();

	d = getDouble(low, hi);

	if (array == NULL) {
		printf("\n\nNull Pointer Exception (op_aastore)\n");
		return;
	}
	if (index >= array->size || index < 0)
	{
	    printf("\n\nNull Pointer Exception (op_aastore)\n");
		return;
	}

	c_array = (double *)arrey_ref->reference;
	c_array[index] = d;
}

void func_dcmpg(){
}

void func_dcmpl(){
}

void func_dconst_0(){
}

void func_dconst_1(){
	u4 a1,a2;
	double d;

	a1 = 0x3FF00000;
	a2 = 0x00000000;
	pushOperand(a2);
	pushOperand(a1);
}

void func_ddiv(){
}

void func_dload(){
}

void func_dload_0(){
}

void func_dload_1(){
  u4 a1 = frame_stack->frame->local_variables[2];
  pushOperand(a1);
  a1 = frame_stack->frame->local_variables[1];
  pushOperand(a1);
}

void func_dload_2(){
}

void func_dload_3(){
}

void func_dmul(){
}

void func_dneg(){
	u4 hi,low;
	u4 a1 = 0xf;

	hi = popOperand();
	low = popOperand();

	hi ^= a1;

	pushOperand(low);
	pushOperand(hi);
}

void func_drem(){
}

void func_dreturn(){
}

void func_dstore(){
}

void func_dstore_0(){
	u4 hi,low;
	u4 a1 = 0xf;

	hi = popOperand();
	low = popOperand();

	frame_stack->frame->local_variables[0] = hi;
	frame_stack->frame->local_variables[1] = low;
}

void func_dstore_1(){
}

void func_dstore_2(){
}

void func_dstore_3(){
}

void func_dsub(){
	u4 hi,low;
	u4 a1 = 0xf;
	double d1,d2;

	hi  = popOperand();
	low = popOperand();
	d1  = getDouble(low,hi);

	hi  = popOperand();
	low = popOperand();
	d2  = getDouble(low,hi);

    d1 = d2 - d1;

    pushOperand(getDoubleHighBytes(d1));
    pushOperand(getDoubleLowBytes(d1));
}

void func_dup(){
}

void func_dup_x1(){
}

void func_dup_x2(){
}

void func_dup2(){
	u4 a1 , a2;

	a1 = popOperand();
	a2 = popOperand();

	pushOperand(a2);
	pushOperand(a1);
	pushOperand(a2);
	pushOperand(a1);
}

void func_dup2_x1(){
}

void func_dup2_x2(){
}


void func_f2d(){
}

void func_f2i(){
    float f;
    u4 a1;

    a1 = popOperanda();
	memcpy(&f, &a1, sizeof(float));
	a1 = (int)f;
	pushOperand(a1);
}

void func_f2l(){
}

void func_fadd(){
}

void func_faload(){
}

void func_fastore(){
	u4 index, value;
    arrays_t  *array;
    float *c_array;
    float f;

	value = popOperand();
	index = popOperand();
	array = (arrays_t *)popOperand();
    memcpy(&f, &value, sizeof(float));

	if (array == NULL) {
		printf("\n\nNull Pointer Exception (func_fastore)\n");
		return;
	}
	if (index >= array->size || index < 0)
	{
	    printf("\n\nNull Pointer Exception (func_fastore)\n");
		return;
	}

	c_array = (float *)arrey_ref->reference;
	c_array[index] = f;

}

void func_fcmpg(){
}

void func_fcmpl(){
}

void func_fconst_0(){
}

void func_fconst_1(){
}

void func_fconst_2(){
}

void func_fdiv(){
}

void func_fload(){
}

void func_fload_0(){
}

void func_fload_1(){
}

void func_fload_2(){
}

void func_fload_3(){
}

void func_fmul(){
	u4 a1, a2;
    float f1, f2;

   a1 = popOperand();
   a2 = popOperand();
   memcpy(&f1, &a1, sizeof(float));
   memcpy(&f2, &a2, sizeof(float));

   f1 *= f2;
   memcpy(&a1, &f1, sizeof(u4));

   pushOperand(a1);
}

void func_fneg(){
}

void func_frem(){
}

void func_freturn(){
}

void func_fstore(){
	u4 a1;
	u2 index;

    frame_stack->frame->pc++;
    index = frame_stack->frame->method->attributes->attribute_union->code[frame_stack->frame->pc];

	a1 = popOperand();
	frame_stack->frame->local_variables[index] = a1;
}

void func_fstore_0(){
}

void func_fstore_1(){
}

void func_fstore_2(){
}

void func_fstore_3(){
}

void func_fsub(){
}

void func_getfield(){
}

void func_getstatic(){
}

void func_goto(){
	unsigned char indexbyte1;
	unsigned char indexbyte2;
	u2 index;

	frame_stack->frame->pc++;
    indexbyte1 = (unsigned char)frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
	frame_stack->frame->pc++;
	indexbyte2 = (unsigned char)frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
	index = indexbyte1 << 8 | indexbyte2;

	frame_stack->frame->pc += index;
	frame_stack->frame->pc--;
}

void func_goto_w(){
}


void func_i2b(){
}

void func_i2c(){
}

void func_i2d(){
	u4 a1, hi,low;
	double d;

	a1 = popOperand();
    d = (double)a1;

    hi = getDoubleHighBytes(d);
    low = getDoubleLowBytes(d);

    pushOperand(low);
    pushOperand(hi);
}

void func_i2f(){
}

void func_i2l(){
}

void func_i2s(){
}

void func_iadd(){
	int op1 = popOperand();
	int op2 = popOperand();
	int result;

	result = op1+op2;

	pushOperand(result);
}

void func_iaload(){
}

void func_iand(){
}

void func_iastore(){
}

void func_iconst_m1(){
	pushOperand(-1);
}

void func_iconst_0(){
	pushOperand(0);
}

void func_iconst_1(){
	pushOperand(1);
}

void func_iconst_2(){
	pushOperand(2);
}

void func_iconst_3(){
	pushOperand(3);
}

void func_iconst_4(){
	pushOperand(4);
}

void func_iconst_5(){
	pushOperand(5);
}

void func_idiv(){
}

void func_if_acmpeq(){

}

void func_if_acmpne(){

}

void func_if_icmpeq(){
}

void func_if_icmpne(){
}

void func_if_icmplt(){
}

void func_if_icmpge(){
}

void func_if_icmpgt(){
}

void func_if_icmple(){
}

void func_ifeq(){
}

void func_ifne(){
}

void func_iflt(){
}

void func_ifge(){
}

void func_ifgt(){
}

void func_ifle(){
}

void func_ifnonnull(){
}

void func_ifnull(){
}

void func_iinc(){
}

void func_iload(){
	u2 index, index2;
	int value;

	frame_stack->frame->pc++;
	index = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
	if (wide){
		frame_stack->frame->pc++;
		index2 = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
		index = ((index << 8)|index2);
		wide = 0;
	}

	value = frame_stack->frame->local_variables[index];

	pushOperand(value);
}

void func_iload_0(){
	int value;
	value = frame_stack->frame->local_variables[0];
	pushOperand(value);
}

void func_iload_1(){
	int value;
	 value = frame_stack->frame->local_variables[1];
	 pushOperand(value);
}

void func_iload_2(){
	int value;
	 value = frame_stack->frame->local_variables[2];
	 pushOperand(value);
}

void func_iload_3(){
	int value;
	 value = frame_stack->frame->local_variables[3];
	 pushOperand(value);
}

void func_imul(){
}

void func_ineg(){
}

void func_instanceof(){
}

void func_invokeinterface(){
}

void func_invokespecial(){
	func_invokevirtual();
}

void func_invokestatic(){
	unsigned char indexbyte1;
	unsigned char indexbyte2;
	char *class_name, *method_name, *method_descriptor;
	cp_info method_ref_info, class_info, method_name_type_ref_info;
	method_info *invoke_method;
	frame_t *frame;
	u4 operand,i;
	u2 index, parameter_count;
	class *resolved_class;

	frame_stack->frame->pc++;
	indexbyte1 = (unsigned char)frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
	frame_stack->frame->pc++;
	indexbyte2 = (unsigned char)frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
	index = indexbyte1 << 8 | indexbyte2;

	method_ref_info = getConstanPoolElement(index);

	/*Pegando o nome da classe*/
	class_info = getConstanPoolElement(method_ref_info.constant_union.c_methodref.class_index);
	class_name = (char *)getConstanPoolElement(class_info.constant_union.c_class.name_index).constant_union.c_utf8.bytes;

	/*Pegando o nome e o descritor do método*/
	method_name_type_ref_info = getConstanPoolElement(method_ref_info.constant_union.c_methodref.name_and_type_index);
	method_name = (char *)getConstanPoolElement(method_name_type_ref_info.constant_union.c_nametype.name_index).constant_union.c_utf8.bytes;
	method_descriptor = (char *)getConstanPoolElement(method_name_type_ref_info.constant_union.c_nametype.descriptor_index).constant_union.c_utf8.bytes;

	resolved_class = getSymbolicReferenceClass(class_name);
	if (resolved_class == NULL) {
		printf("ClassNotFoundException!\n");
		exit(0);
	}

	/*Metodo a ser invocado*/
	invoke_method = getMethod(resolved_class->class_file, method_name, method_descriptor);
	if (invoke_method == NULL) {
		printf("MethodNotFoundException!\n");
		exit(0);
	}

	parameter_count = getParameterCount(method_descriptor);

	frame = createFrame(invoke_method, resolved_class->class_file->constant_pool, 0);

	/*Devo empilhar ao contrário, pois a pilha vai conter a object reference por ultimo.
	Comeca com 1 por causa do indice do vetor*/
	for (i=1; i <= parameter_count; i++) {
		operand = popOperand();
		frame->local_variables[parameter_count-i] = operand;
	}

	pushFrame(frame);
}

void func_invokevirtual(){

	unsigned char indexbyte1;
	unsigned char indexbyte2;
	char *class_name, *method_name, *method_descriptor;
	cp_info method_ref_info, class_info, method_name_type_ref_info;
	method_info *invoke_method;
	frame_t *frame;
	u4 operand,i;
	u2 index, parameter_count;
	class *resolved_class;

	frame_stack->frame->pc++;
	indexbyte1 = (unsigned char)frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
	frame_stack->frame->pc++;
	indexbyte2 = (unsigned char)frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
	index = indexbyte1 << 8 | indexbyte2;

	method_ref_info = getConstanPoolElement(index);

	/*Pegando o nome da classe*/
	class_info = getConstanPoolElement(method_ref_info.constant_union.c_methodref.class_index);
	class_name = (char *)getConstanPoolElement(class_info.constant_union.c_class.name_index).constant_union.c_utf8.bytes;

	/*Pegando o nome e o descritor do método*/
	method_name_type_ref_info = getConstanPoolElement(method_ref_info.constant_union.c_methodref.name_and_type_index);
	method_name = (char *)getConstanPoolElement(method_name_type_ref_info.constant_union.c_nametype.name_index).constant_union.c_utf8.bytes;
	method_descriptor = (char *)getConstanPoolElement(method_name_type_ref_info.constant_union.c_nametype.descriptor_index).constant_union.c_utf8.bytes;

	resolved_class = getSymbolicReferenceClass(class_name);
	if (resolved_class == NULL) {
		printf("ClassNotFoundException!\n");
		exit(0);
	}

	/*Metodo a ser invocado*/
	invoke_method = getMethod(resolved_class->class_file, method_name, method_descriptor);
	if (invoke_method == NULL) {
		printf("MethodNotFoundException!\n");
		exit(0);
	}

	parameter_count = getParameterCount(method_descriptor);

	frame = createFrame(invoke_method, resolved_class->class_file->constant_pool, 0);

	/*Devo empilhar ao contrário, pois a pilha vai conter a object reference por ultimo.
	Comeca com 1 por causa do indice do vetor*/
	for (i=1; i <= parameter_count; i++) {
		operand = popOperand();
		frame->local_variables[parameter_count-i] = operand;
	}

	pushFrame(frame);

}

void func_ior(){
}

void func_irem(){
	u4 a1,a2;

	a2 = popOperand();
	a1 = popOperand();

	a1 = a1 - (a1/a2)*a2;
	pushOperand(a1);
}

void func_ireturn(){
}

void func_ishl(){
	int aux1, aux2;

	aux2 = (int)pop();
	aux1 = (int)pop();
	aux2 = aux2 & 0x1f;
	pushOperand(aux1 << aux2);
}

void func_ishr(){
}

void func_istore(){
	int value = popOperand();
	u2 index;

	frame_stack->frame->pc++;
	index = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
	frame_stack->frame->local_variables[index] = value;
}

void func_istore_0(){
	int value = popOperand();
	frame_stack->frame->local_variables[0] = value;
}

void func_istore_1(){
	int value = popOperand();
	frame_stack->frame->local_variables[1] = value;
}

void func_istore_2(){
	int value = popOperand();
	frame_stack->frame->local_variables[2] = value;
}

void func_istore_3(){
	int value = popOperand();
	frame_stack->frame->local_variables[3] = value;
}

void func_isub(){
}

void func_iushr(){
	u4 aux1, aux2;

	aux2 = pop();
	aux1 = pop();
	aux2 = aux2 & 0x1f;
	pushOperand(aux1 >> aux2);
}

void func_ixor(){
}


void func_jsr(){

}

void func_jsr_w(){
}


void func_l2d(){
	long long aux1, aux3, aux4;
	double aux2;
	u4 u4aux;

	aux4 = pop();
	aux1 = pop();
	aux1 = aux1 << 32;
	aux1 |= aux4;
	aux2 = (double)aux1;
	memcpy(&aux3, &aux2, sizeof(int64_t));
	u4aux = aux3 >> 32;
	pushOperand(u4aux);
	u4aux = aux3 & 0xffffffff;
	pushOperand(u4aux);
}

void func_l2f(){
}

void func_l2i(){
}

void func_ladd(){
}

void func_laload(){
	arrays_t *arrey_ref;
	u4 index;
    u8 *array;
    u4 hi, low;

	index = popOperand();
	arrey_ref = (arrays_t *)popOperand();

	if (array == NULL) {
		printf("\n\nNull Pointer Exception (op_aastore)\n");
		return;
	}
	if (index >= array->size)
	{
	    printf("\n\nNull Pointer Exception (op_aastore)\n");
		return;
	}

	array = (u8 *)arrey_ref->reference;

	hi = (array[index]>>32);
	low = (array[index] & 0xffffffff);

	pushOperand(low);
	pushOperand(hi);
}

void func_land(){
}

void func_lastore(){
}

void func_lcmp(){
}

void func_lconst_0(){
	arrays_t *arrey_ref;
	u4 index;
    u8 *array;
    u4 hi, low;

	index = popOperand();
	arrey_ref = (arrays_t *)popOperand();

	hi = (array[index]>>32);
	low = (array[index] & 0xffffffff);

	pushOperand(low);
	pushOperand(hi);
}

void func_lconst_1(){
}

void func_ldc(){
}

void func_ldc_w(){
}

void func_ldc2_w(){
}

void func_ldiv(){
}

void func_lload(){
}

void func_lload_0(){
}

void func_lload_1(){
}

void func_lload_2(){
}

void func_lload_3(){
}

void func_lmul(){
}

void func_lneg(){
	u4 a2, a1;
	int64_t d;

	a1 = pop();
	d = (signed)pop();
	d = d << 32;
	d |= a1;
	d = -d;
	a2 = d >> 32;
	push(a2);
	a2 = d & 0xffffffff;
	push(a2);
	frame_stack->pc++;
}

void func_lookupswitch(){
}

void func_lor(){
}

void func_lrem(){
}

void func_lreturn(){
}

void func_lshl(){
}

void func_lshr(){
}

void func_lstore(){
}

void func_lstore_0(){
}

void func_lstore_1(){
}

void func_lstore_2(){
}

void func_lstore_3(){
}

void func_lsub(){
	u4 a2, a1;
	int64_t d, d2;

	a1 = pop();
	d = (signed)pop();
	d = d << 32;
	d |= a1;
	d = -d;
	a2 = d >> 32;
	push(a2);
	a2 = d & 0xffffffff;
	push(a2);
	frame_stack->pc++;
}

void func_lushr(){
}

void func_lxor(){
}


void func_monitorenter(){
}

void func_monitorexit(){
}

void func_multianewarray(){
}


void func_new(){
}

void func_newarray(){
	unsigned char indexbyte1;
	unsigned char indexbyte2;
    arrays_t *array;
	u2 index;
	u4 count;
	u1 type;

	frame_stack->frame->pc++;
	type = frame_stack->frame->method->attributes->attribute_union.code.code[frame_stack->frame->pc];
	count = (u4)popOperand();

	array = calloc(1, sizeof(arrays_t));
	array->size = count;
	array->type = type;
	switch (type)
	{
		case 4:
			array->reference = calloc(count, sizeof(char));
		case 5:
			array->reference = calloc(count, sizeof(char));
		case 6:
			array->reference = calloc(count, sizeof(float));
		case 7:
			array->reference = calloc(count, sizeof(double));
		case 8:
			array->reference = calloc(count, sizeof(unsigned char));
		case 9:
			array->reference = calloc(count, sizeof(short int));
		case 10:
			array->reference = calloc(count, sizeof(int));
		case 11:
			array->reference = calloc(count, sizeof(long long));
	}


	push(array);
}

void func_nop(){
}


void func_pop(){
}

void func_pop2(){
}

void func_putfield(){
}

void func_putstatic(){
}


void func_ret(){

}

void func_return(){
	popFrame();
}


void func_saload(){
}

void func_sastore(){
}

void func_sipush(){
}

void func_swap(){
}


void func_tableswitch(){
}


void func_wide(){
	wide=1;
}


void func_breakpoint(){
}

void func_impdep1(){
}

void func_impdep2(){
}

