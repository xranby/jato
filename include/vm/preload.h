#ifndef _VM_JAVA_LANG_H
#define _VM_JAVA_LANG_H

extern struct vm_class *vm_array_of_java_lang_Class;
extern struct vm_class *vm_array_of_java_lang_StackTraceElement;
extern struct vm_class *vm_array_of_java_lang_String;
extern struct vm_class *vm_array_of_java_lang_reflect_Constructor;
extern struct vm_class *vm_array_of_java_lang_reflect_Field;
extern struct vm_class *vm_array_of_java_lang_reflect_Method;
extern struct vm_class *vm_java_lang_Object;
extern struct vm_class *vm_java_lang_Class;
extern struct vm_class *vm_java_lang_Cloneable;
extern struct vm_class *vm_java_lang_String;
extern struct vm_class *vm_java_lang_Throwable;
extern struct vm_class *vm_java_util_Properties;
extern struct vm_class *vm_java_lang_VMThrowable;
extern struct vm_class *vm_java_lang_StackTraceElement;
extern struct vm_class *vm_java_lang_Error;
extern struct vm_class *vm_java_lang_OutOfMemoryError;
extern struct vm_class *vm_java_lang_InternalError;
extern struct vm_class *vm_java_lang_ArithmeticException;
extern struct vm_class *vm_java_lang_NullPointerException;
extern struct vm_class *vm_java_lang_NegativeArraySizeException;
extern struct vm_class *vm_java_lang_ClassCastException;
extern struct vm_class *vm_java_lang_ClassNotFoundException;
extern struct vm_class *vm_java_lang_NoClassDefFoundError;
extern struct vm_class *vm_java_lang_UnsatisfiedLinkError;
extern struct vm_class *vm_java_lang_ArrayIndexOutOfBoundsException;
extern struct vm_class *vm_java_lang_ArrayStoreException;
extern struct vm_class *vm_java_lang_RuntimeException;
extern struct vm_class *vm_java_lang_ExceptionInInitializerError;
extern struct vm_class *vm_java_lang_NoSuchFieldError;
extern struct vm_class *vm_java_lang_NoSuchMethodError;
extern struct vm_class *vm_java_lang_StackOverflowError;
extern struct vm_class *vm_java_lang_Thread;
extern struct vm_class *vm_java_lang_ThreadGroup;
extern struct vm_class *vm_java_lang_VMThread;
extern struct vm_class *vm_java_lang_IllegalMonitorStateException;
extern struct vm_class *vm_java_lang_reflect_Constructor;
extern struct vm_class *vm_java_lang_reflect_Field;
extern struct vm_class *vm_java_lang_reflect_Method;
extern struct vm_class *vm_java_lang_reflect_VMConstructor;
extern struct vm_class *vm_java_lang_reflect_VMField;
extern struct vm_class *vm_java_lang_reflect_VMMethod;
extern struct vm_class *vm_java_lang_Byte;
extern struct vm_class *vm_java_lang_Boolean;
extern struct vm_class *vm_java_lang_Character;
extern struct vm_class *vm_java_lang_Double;
extern struct vm_class *vm_java_lang_Float;
extern struct vm_class *vm_java_lang_Integer;
extern struct vm_class *vm_java_lang_Long;
extern struct vm_class *vm_java_lang_Short;
extern struct vm_class *vm_java_lang_IllegalArgumentException;
extern struct vm_class *vm_java_lang_ClassLoader;
extern struct vm_class *vm_java_lang_Number;
extern struct vm_class *vm_java_lang_InterruptedException;
extern struct vm_class *vm_java_lang_ClassFormatError;
extern struct vm_class *vm_boolean_class;
extern struct vm_class *vm_char_class;
extern struct vm_class *vm_float_class;
extern struct vm_class *vm_double_class;
extern struct vm_class *vm_byte_class;
extern struct vm_class *vm_short_class;
extern struct vm_class *vm_int_class;
extern struct vm_class *vm_long_class;
extern struct vm_class *vm_void_class;

extern struct vm_field *vm_java_lang_Class_vmdata;
extern struct vm_field *vm_java_lang_String_offset;
extern struct vm_field *vm_java_lang_String_count;
extern struct vm_field *vm_java_lang_String_value;
extern struct vm_field *vm_java_lang_Throwable_detailMessage;
extern struct vm_field *vm_java_lang_VMThrowable_vmdata;
extern struct vm_field *vm_java_lang_Thread_daemon;
extern struct vm_field *vm_java_lang_Thread_group;
extern struct vm_field *vm_java_lang_Thread_name;
extern struct vm_field *vm_java_lang_Thread_priority;
extern struct vm_field *vm_java_lang_Thread_contextClassLoader;
extern struct vm_field *vm_java_lang_Thread_contextClassLoaderIsSystemClassLoader;
extern struct vm_field *vm_java_lang_Thread_vmThread;
extern struct vm_field *vm_java_lang_VMThread_thread;
extern struct vm_field *vm_java_lang_VMThread_vmdata;
extern struct vm_field *vm_java_lang_reflect_Constructor_clazz;
extern struct vm_field *vm_java_lang_reflect_Constructor_cons;
extern struct vm_field *vm_java_lang_reflect_Constructor_slot;
extern struct vm_field *vm_java_lang_reflect_Field_declaringClass;
extern struct vm_field *vm_java_lang_reflect_Field_f;
extern struct vm_field *vm_java_lang_reflect_Field_name;
extern struct vm_field *vm_java_lang_reflect_Field_slot;
extern struct vm_field *vm_java_lang_reflect_Method_declaringClass;
extern struct vm_field *vm_java_lang_reflect_Method_m;
extern struct vm_field *vm_java_lang_reflect_Method_name;
extern struct vm_field *vm_java_lang_reflect_Method_slot;
extern struct vm_field *vm_java_lang_reflect_VMConstructor_clazz;
extern struct vm_field *vm_java_lang_reflect_VMConstructor_slot;
extern struct vm_field *vm_java_lang_reflect_VMField_clazz;
extern struct vm_field *vm_java_lang_reflect_VMField_name;
extern struct vm_field *vm_java_lang_reflect_VMField_slot;
extern struct vm_field *vm_java_lang_reflect_VMMethod_clazz;
extern struct vm_field *vm_java_lang_reflect_VMMethod_name;
extern struct vm_field *vm_java_lang_reflect_VMMethod_slot;
extern struct vm_field *vm_java_lang_reflect_VMMethod_m;
extern struct vm_field *vm_java_lang_ClassLoader_systemClassLoader;

extern struct vm_method *vm_java_util_Properties_setProperty;
extern struct vm_method *vm_java_lang_Throwable_initCause;
extern struct vm_method *vm_java_lang_Throwable_getCause;
extern struct vm_method *vm_java_lang_Throwable_stackTraceString;
extern struct vm_method *vm_java_lang_Throwable_getStackTrace;
extern struct vm_method *vm_java_lang_Throwable_setStackTrace;
extern struct vm_method *vm_java_lang_StackTraceElement_init;
extern struct vm_method *vm_java_lang_Thread_init;
extern struct vm_method *vm_java_lang_Thread_isDaemon;
extern struct vm_method *vm_java_lang_Thread_getName;
extern struct vm_method *vm_java_lang_ThreadGroup_init;
extern struct vm_method *vm_java_lang_ThreadGroup_addThread;
extern struct vm_method *vm_java_lang_VMThread_init;
extern struct vm_method *vm_java_lang_VMThread_run;
extern struct vm_method *vm_java_lang_System_exit;
extern struct vm_method *vm_java_lang_Boolean_init;
extern struct vm_method *vm_java_lang_Boolean_valueOf;
extern struct vm_method *vm_java_lang_Byte_init;
extern struct vm_method *vm_java_lang_Byte_valueOf;
extern struct vm_method *vm_java_lang_Character_init;
extern struct vm_method *vm_java_lang_Character_valueOf;
extern struct vm_method *vm_java_lang_Double_init;
extern struct vm_method *vm_java_lang_Double_valueOf;
extern struct vm_method *vm_java_lang_Float_init;
extern struct vm_method *vm_java_lang_Float_valueOf;
extern struct vm_method *vm_java_lang_InheritableThreadLocal_newChildThread;
extern struct vm_method *vm_java_lang_Integer_init;
extern struct vm_method *vm_java_lang_Integer_valueOf;
extern struct vm_method *vm_java_lang_Long_init;
extern struct vm_method *vm_java_lang_Long_valueOf;
extern struct vm_method *vm_java_lang_Short_init;
extern struct vm_method *vm_java_lang_Short_valueOf;
extern struct vm_method *vm_java_lang_ClassLoader_loadClass;
extern struct vm_method *vm_java_lang_ClassLoader_getSystemClassLoader;
extern struct vm_method *vm_java_lang_Number_intValue;
extern struct vm_method *vm_java_lang_Number_floatValue;
extern struct vm_method *vm_java_lang_Number_longValue;
extern struct vm_method *vm_java_lang_Number_doubleValue;

int preload_vm_classes(void);

#endif
