/* 
 *  Simple Virtual Machine - A versatile and robust architecture to
 *  easily write applications.
 *  Copyright (C) 2021  Julien BRUGUIER
 * 
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the Lesser GNU General Public License as published by
 *  the Free Software Foundation; either version 3 of the License, or
 *  (at your option) any later version.
 * 
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 * 
 *  You should have received a copy of the Lesser GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SVM_API
#define SVM_API

/** \mainpage Simple Virtual Machine API
 *
 * This header contains all needed functions to alter the virtual machine from plugins written for it.
 *
 * These plugins allow users to improve the architecture of the machine by defining their own:
 * - instructions: allows extension of the SVM processors instruction set,
 * - memory types: allows extension of the SVM memory storage capacity,
 * - interruptions: allows dedicated interruptions of a processor,
 * - sequencers: allows extension of the sequencing of kernels with processes,
 * - schedulers: allows extension of the orchestration of processes within the virtual machine,
 * - functions: allows inter plugin communication by acting like private instructions,
 * - structures: also allows inter plugin communication by acting like private types.
 *
 * Such objects are defined by a (rather long) string describing each of them and their dependencies on other plugin objects by a call to the svm_plugin_configure API special function.
 *
 * Associated to each object and to give them life, some particular plugin callback functions have to be defined:
 * - For instructions:
 *   \code{.c}
 *   SVM_Value instruction_<instruction name>(const void *svm, SVM_Size argc, SVM_Parameter argv[]); // mandatory
 *   \endcode
 * - For types:
 *   \code{.c}
 *   void type_<type name>_delete(const void *svm, void *handler); // mandatory
 *   void* type_<type name>_copy(const void *svm, const void *handler); // optional
 *   void* type_<type name>_constant(const void *svm, const SVM_String value); // optional
 *   SVM_String type_<type name>_print(const void *svm, const void *handler); // optional
 *   \endcode
 * - For interruptions:
 *   No plugin callback to be defined.
 * - For sequencers:
 *   \code{.c}
 *   void* sequencer_<sequencer name>_create(const void *svm); // mandatory
 *   void sequencer_<sequencer name>_delete(const void *svm, const void *handler); // mandatory
 *   SVM_Kernel sequencer_<sequencer name>_current(const void *svm, const void *handler); // mandatory
 *   SVM_Boolean sequencer_<sequencer name>_attach(const void *svm, const void *handler, SVM_Kernel kernel, SVM_Size argc, SVM_Parameter argv[]); // mandatory
 *   SVM_Boolean sequencer_<sequencer name>_detach(const void *svm, const void *handler, SVM_Kernel kernel, SVM_Size argc, SVM_Parameter argv[]); // mandatory
 *   SVM_String sequencer_<sequencer name>_print(const void *svm, const void *handler); // mandatory
 *   \endcode
 * - For schedulers:
 *   \code{.c}
 *   void* scheduler_<scheduler name>_create(const void *svm); // mandatory
 *   void scheduler_<scheduler name>_delete(const void *svm, const void *handler); // mandatory
 *   unsigned long int scheduler_<scheduler name>_schedule(const void *svm, const void *handler, const SVM_Process process, const SVM_Process_State state); // mandatory
 *   unsigned long int scheduler_<scheduler name>_notification(const void *svm, const void *handler, const SVM_Notification_Type type, const unsigned long int parameter); // optional
 *   SVM_Boolean scheduler_<scheduler name>_attach(const void *svm, const void *handler, const SVM_Process process, unsigned long int parameter); // mandatory
 *   SVM_Boolean scheduler_<scheduler name>_detach(const void *svm, const void *handler, const SVM_Process process, unsigned long int parameter); // mandatory
 *   SVM_String scheduler_<scheduler name>_print(const void *svm, const void *handler); // mandatory
 *   \endcode
 * - For functions:
 *   \code{.c}
 *   SVM_Variable function_<function name>(const void *svm, SVM_Size argc, SVM_Parameter argv[]); // mandatory
 *   \endcode
 * - For structures:
 *   \code{.c}
 *   void struct_<struct name>_delete(const void *svm, void *handler); // mandatory
 *   void* struct_<struct name>_copy(const void *svm, const void *handler); // optional
 *   \endcode
 *
 * Please refer to the svm manpage to get details about the call, parameters and expected return values of these callbacks.
 *
 * To let the plugin callbacks interact with the SVM, some API functions are defined in this file.
 *
 * \note API functions have some default behavior:
 * - when an invalid condition is reached in the API function, it will interrupt the full plugin callback and also the calling program.
 *   To avoid interruptions, other API functions can be called to detect errors conditions.
 * - when a parameter is a pointer and is passed NULL, a FAILURE interruption will be raised with a message complaining about a void parameter,
 * - when a parameter is a pointer that has not been returned by a previous API function call or coming from a callback parameter,
 *   a FAILURE interruption will be raised with a message complaining about an invalid parameter,
 * - when a parameter is a pointer that has been returned by a previous API function call or a callback parameter but has an invalid type,
 *   a FAILURE interruption will be raised with a message complaining about a parameter not being of the wanted type,
 * - when a function returns a SVM_<something> pointer or a SVM_String structure, never free or delete the pointer or the buffer.
 *   The SVM will automatically manage the life time of objects created in plugin callbacks.
 *  (Explicit void* are not concerned and have to be freed manually.)
 *
 * \warning Freeing or deleting a pointer returned by an API function will result in an undefined behavior.
 *
 * \note Any API function requires a pointer as first parameter. This parameter is given as first parameter of the plugin callback.
 * 
 * \warning Passing to an API function a first parameter not being the first one of a plugin callback will result in an undefined behavior.
 * Any modification attempt to this pointer will also result in an undefined behavior.
 *
 * \note When a function requires a SVM_Kernel or a SVM_Process, by default if the provided SVM_Kernel or SVM_Process is not the current process,
 * the given process has to be locked by the current process to achieve its action. A FAILURE interruption will be raised otherwise.
 * A lock can be obtained by the svm_process_ownership_lock API function.
 *
 * \see configuration
 * \see svm_process_ownership_lock
 *
 */

#ifdef __cplusplus
extern "C"
{
#endif

/** \defgroup apimacros SVM API macros
 * This module contains API macros.
 * \{
 */

/**
 * Declares a monoline SVM type.
 */
#define SVM_TYPE
/**
 * Declares the begining of a multiline SVM type.
 */
#define SVM_TYPE_BEGIN
/**
 * Declares the end of a multiline SVM type.
 */
#define SVM_TYPE_END
/**
 * Declares a monoline SVM API function.
 */
#define SVM_FUNCTION

/** \} */

/** \defgroup api_types SVM API types
 * This module contains all types used by API functions.
 *
 * Some are only enumeration constants used in various ways, like named booleans or SVM elements states or types.
 *
 * One is a buffer containers to give a access to non-nul terminated strings, sometimes useful while conveying binary data
 * over virtual machine strings.
 *
 * The others ones are all typedefs on:
 * \code{.c}
 * const void *
 * \endcode
 * These are various variable types:
 * - A parameter with SVM_Variable type can accept a variable of any other type,
 * - A parameter with SVM_Value type can accept a variable of any type starting with SVM_Value_ prefix.
 *
 * \see variables
 * \{
 */

/*
 * These types correspond to specific objects used in this API.
 */

/** \typedef SVM_Variable
 * This type is used to represent any object managed by the SVM.
 *
 * \see variables
 */
SVM_TYPE typedef const void* SVM_Variable;

/** \typedef SVM_Parameter
 * This type is used by the SVM to pass instruction parameters to the corresponding plugin function.
 *
 * It is also used to pass parameters to plugin function calls.
 *
 * \see variables
 */
SVM_TYPE typedef const void* SVM_Parameter;

/** \typedef SVM_Structure
 * This type is used to convey a C pointer used for data exchange between plugins.
 *
 * \see variables
 */
SVM_TYPE typedef const void* SVM_Structure;

/** \typedef SVM_AccessControl
 * This type is used to represent quotas on code execution.
 *
 * \see variables
 */
SVM_TYPE typedef const void* SVM_AccessControl;

/** \typedef SVM_Memory_Zone
 * This type is used to represent the types of a contiguous zone of memory prior to its allocation.
 *
 * \see variables
 */
SVM_TYPE typedef const void* SVM_Memory_Zone;

/** \typedef SVM_Lock
 * This type is used as a simple synchronisation facility.
 *
 * This lock mutually excludes a unique writter against several concurrent readers.
 *
 * \see variables
 */
SVM_TYPE typedef const void* SVM_Lock;

/** \typedef SVM_LockGuard_Read
 * This type is used to represent the aquisition of a SVM_Lock as a reader.
 *
 * \see variables
 */
SVM_TYPE typedef const void* SVM_LockGuard_Read;

/** \typedef SVM_LockGuard_Write
 * This type is used to represent the aquisition of a SVM_Lock as a writer.
 *
 * \see variables
 */
SVM_TYPE typedef const void* SVM_LockGuard_Write;

/** \typedef SVM_Event_Queue
 * This type is used as a complex synchronisation facility.
 *
 * It allows synchronisation of several clients through event exchange.
 *
 * \see variables
 */
SVM_TYPE typedef const void* SVM_Event_Queue;

/** \typedef SVM_Event_Queue_Address
 * This type is used to represent an entity able to post and get events from an event queue.
 *
 * \see variables
 */
SVM_TYPE typedef const void* SVM_Event_Queue_Address;

/** \typedef SVM_Address
 * This type is used to represent:
 * - either a local code address,
 * - or a memory address.
 */
SVM_TYPE typedef unsigned long int SVM_Address;

/** \typedef SVM_Index
 * This type is used to represent an index.
 */
SVM_TYPE typedef unsigned long int SVM_Index;

/** \typedef SVM_Size
 * This type is used to represent an object size.
 */
SVM_TYPE typedef unsigned long int SVM_Size;

/** \typedef SVM_Code
 * This type is used to represent a section of code of the SVM.
 *
 * It consists in:
 * - a name for debugging purpose,
 * - a list of instructions,
 * - a local anchor mapping (built from :label instructions),
 * - a global anchor mapping (built from :symbol instructions).
 *
 * \see variables
 */
SVM_TYPE typedef const void* SVM_Code;

/** \typedef SVM_Kernel
 * This type is used to represent an atomic execution environment.
 *
 * It consists in:
 * - a state,
 * - a processor,
 * - a memory.
 *
 * In this API, functions managing processors and memories access them through the kernel holding them.
 *
 * \see variables
 */
SVM_TYPE typedef const void* SVM_Kernel;

/** \typedef SVM_Process
 * This type is used to represent an execution thread in the SVM.
 *
 * It consists in:
 * - a name for debugging purpose,
 * - a state,
 * - a sequencer deciding in which order the kernels are executed,
 * - a list of kernels.
 *
 * In this API:
 * - functions managing the sequencer access it through the process holding it,
 * - functions managing a process will check whether the process can be safely managed:
 *    - the action is done on the current process,
 *    - the action is done on another process: in such case, the process has to be locked by the current one,
 *    - for few functions, this lock is not needed even on other processes, and this will be mentionned in the help of these functions.
 *
 * \see variables
 */
SVM_TYPE typedef const void* SVM_Process;

/** \typedef SVM_Process_Lock
 * This type is used to represent the aquisition of a lock on a remote process.
 *
 * \see variables
 */
SVM_TYPE typedef const void* SVM_Process_Lock;

/** \typedef SVM_Scheduler
 * This type is used to represent an execution scheduler.
 *
 * A scheduler is a process execution orchestrator.
 *
 * \see variables
 */
SVM_TYPE typedef const void* SVM_Scheduler;

/** \typedef SVM_Debug_Form
 * This type is used to represent the a formular in the debugger user interface.
 *
 * \see variables
 */
SVM_TYPE typedef const void* SVM_Debug_Form;

/** \typedef SVM_String
 * This type corresponds to a sized string for C plugins.
 */
SVM_TYPE_BEGIN
typedef struct
{
	const char *string; /**< The memory buffer associated to this string is always held by the SVM when it has been returned from an API function.
			     * \warning NEVER FREE OR DELETE A STRING BUFFER RETURNED BY THE SVM. This buffer will be freed/deleted when it is not longer useful.
			     *
			     * \note Under some circonstancies, an extra null byte is added after the buffer size to be compatible with C strings.
			     *
			     * \warning As no buffer underflow or overflow checks are done on the buffer of the string, doing a read or a write outside the buffer will result in an undefined behavior.
			     */
	SVM_Size size;
} SVM_String;
SVM_TYPE_END

/** \enum SVM_Boolean
 * This type corresponds to a simple boolean.
 *
 * This boolean is compatible with C ones.
 */
SVM_TYPE_BEGIN
typedef enum
{
	FALSE = 0, /**< self-explained */
	TRUE = 1 /**< self-explained */
} SVM_Boolean;
SVM_TYPE_END

/** \enum SVM_Type_Internal
 * This type corresponds to value types the SVM can store in its memory.
 *
 * The last one represents the class of types the plugin developpers can define in theirs plugins.
 */
SVM_TYPE_BEGIN
typedef enum
{
	AUTOMATIC, /**< generic type to be replaced at first assignation */
	INTEGER, /**< integer type corresponding to long long int */
	STRING, /**< string type corresponding to SVM_String */
	BOOLEAN, /**< boolean type corresponding to SVM_Boolean */
	POINTER, /**< pointer type used to target memory zones */
	LIBRARY, /**< library type corresponding to SVM_Code  */
	SYMBOL, /**< symbol type used to target code location */
	PLUGIN_ENTRY_POINT, /**< plugin entry point type used to identify a plugin object */
	INTERRUPTION /**< interruption type corresponds to processor interruptions */
} SVM_Type_Internal;
SVM_TYPE_END

/** \enum SVM_Interruption_Internal
 *
 * This type corresponds to interruption types the SVM can use to stop program executions.
 */
SVM_TYPE_BEGIN
typedef enum
{
	CLONE, /**< raised on SIGCHLD */
	DEVICE, /**< raised on SIGPIPE, SIGBUS */
	FAILURE, /**< raised on SIGABRT, SIGQUIT or generic failure in instruction */
	FIRST, /**< raised on SIGUSR1 */
	MEMORY, /**< raised on memory access failure */
	NUMERIC, /**< raised on SIGFPE */
	PROCESSOR, /**< raised on SIGILL or processor invalid action */
	SECOND, /**< raised on SIGUSR2 */
	SECURITY, /**< raised on security violation or quota overflow */
	TERMINATE, /**< raised on SIGTERM, SIGINT, SIGHUP, SIGALRM */
	TERMINAL, /**< raised on SIGTTIN, SIGTTOU, SIGWINCH */
	GENERIC /**< raised on all other signals */
} SVM_Interruption_Internal;
SVM_TYPE_END

/** \enum SVM_Interruption_Kind
 *
 * This type represents the two major classes of interruptions the SVM can handle.
 */
SVM_TYPE_BEGIN
typedef enum
{
	SOFTWARE, /**< the class of interruptions coming from instructions and execution errors */
	HARDWARE /**< the class of interruptions coming from the OS signals */
} SVM_Interruption_Kind;
SVM_TYPE_END

/** \enum SVM_Notification_Type
 *
 * This type represents the reason that triggered a callback call to a specific scheduler function. (See scheduler API functions below for more details.)
 */
SVM_TYPE_BEGIN
typedef enum
{
	NOTIFICATION, /**< the callback has been triggered manually by the plugin developper */
	TIMER /**< the callback has been triggered after a timer expiration */
} SVM_Notification_Type;
SVM_TYPE_END

/** \enum SVM_Process_State
 *
 * This type represents the status of a process.
 */
SVM_TYPE_BEGIN
typedef enum
{
	RUNNING, /**< the process lets the processor of the current kernel executing instructions */
	SUSPENDED, /**< the process is in a light sleep, and can return in running mode anytime */
	WAITING, /**< the process is blocked in a state in which it may not answer to scheduler orders */
	LOCKED, /**< the process is temporarly owned by someone and can not return in running mode */
	DEBUG, /**< the process is blocked in a state it answers only to the debugger */
	ZOMBIE, /**< the process has completed its execution and is not destroyed yet */
	INTERRUPTED, /**< the process has completed its execution on a non-handled interruption */
	ERROR /**< the process reached an invalid state */
} SVM_Process_State;
SVM_TYPE_END

/** \enum SVM_Object_Level
 *
 * This type corresponds to the level of definition of an object.
 */
SVM_TYPE_BEGIN
typedef enum
{
	LOCAL, /**< the object is located in the current state of the processor */
	CASCADE, /**< the object is located in the current state and all sub states of the processor */
	GLOBAL /**< the object is located in the processor */
} SVM_Object_Level;
SVM_TYPE_END

/** \typedef SVM_Comparison_Result
 * This type corresponds to the result of SVM_Value comparison.
 */
SVM_TYPE_BEGIN
typedef struct
{
	SVM_Boolean equal; /**< TRUE when values are equal, FALSE otherwise */
	SVM_Boolean different; /**< TRUE when values are different, FALSE otherwise */
	SVM_Boolean inferior; /**< TRUE when comparison is an order and left value is inferior to right value, FALSE otherwise */
	SVM_Boolean inferior_or_equal; /**< TRUE when comparison is an order and left value is inferior or equal to right value, FALSE otherwise */
	SVM_Boolean superior; /**< TRUE when comparison is an order and left value is superior to right value, FALSE otherwise */
	SVM_Boolean superior_or_equal; /**< TRUE when comparison is an order and left value is superior or equal to right value, FALSE otherwise */
	SVM_Boolean order; /**< TRUE when comparison supports ordering, FALSE when comparison supports only equivalence */
	SVM_Boolean total; /**< TRUE when comparison is an order and this comparison is total, FALSE otherwise */
	SVM_Boolean weak; /**< TRUE when values are not comparables (a fallback arbitrary comparison is used instead), FALSE otherwise */
	SVM_Boolean type; /**< TRUE when values types does not have the same type, FALSE otherwise */
	SVM_Boolean null; /**< TRUE when at least one value was null, FALSE otherwise */
} SVM_Comparison_Result;
SVM_TYPE_END

/** \enum SVM_Value_Plugin_Comparison
 *
 * This type corresponds to the result of SVM_Value_Plugin comparison.
 * It can be used by plugin value comparison callbacks as explicit return value.
 */
SVM_TYPE_BEGIN
typedef enum
{
	EQUIVALENCE_EQUAL = 0x0, /**< Used to specify that values are equal */
	EQUIVALENCE_DIFFERENT = 0x3, /**< Used to specify that values are different */
	ORDER_EQUAL = 0x4, /**< Used to specify that values are equal */
	ORDER_INFERIOR = 0x5, /**< Used to specify that left value is inferior to right value */
	ORDER_SUPERIOR = 0x6, /**< Used to specify that left value is superior to right value */
	ORDER_UNCOMPARABLE = 0x7, /**< Used to specify that left value and the right value can not be compared */
	ORDER_TOTAL = 0x0, /**< Used to specify that the order is total (unused when an equivalence is used) */
	ORDER_PARTIAL = 0x8, /**< Used to specify that the order is partial (unused when an equivalence is used, set automatically when ORDER_UNCOMPARABLE is used) */
	COMPARISON_STRONG = 0x0, /**< Used to specify that the real comparison has been computed */
	COMPARISON_WEAK = 0x10 /**< Used to specify that an heuristic or arbitrary comparison has been computed */
} SVM_Value_Plugin_Comparison;
SVM_TYPE_END

#define SVM_NOTHING /**< \ingroup apimacros Useless for the API. Only here to trick Doxygen... */

/** \typedef SVM_Kernel_State
 * This type represents the status of a kernel 
 */
SVM_TYPE typedef SVM_Process_State SVM_Kernel_State;

/** \typedef SVM_Type
 * This type is used to represent types of any value the SVM can store in one of its memories.
 *
 * \see SVM_Value
 */
SVM_TYPE typedef const void* SVM_Type;

/** \typedef SVM_Value
 * This type is used to represent any value the SVM can store in one of its memories.
 *
 * This type is not equivalent to the automatic type. It just means the type of the value will not be considered from this variable.
 *
 * \see variables
 */
SVM_TYPE typedef const void* SVM_Value;

/** \typedef SVM_Value_Integer
 * This type is used to represent an integer the SVM can store in one of its memories.
 *
 * The corresponding C type is long long int.
 *
 * \see variables
 */
SVM_TYPE typedef const void* SVM_Value_Integer;

/** \typedef SVM_Value_String
 * This type is used to represent a string the SVM can store in one of its memories.
 *
 * The corresponding C type is SVM_String or C-nul terminated strings.
 *
 * \see variables
 */
SVM_TYPE typedef const void* SVM_Value_String;

/** \typedef SVM_Value_Boolean
 * This type is used to represent a boolean the SVM can store in one of its memories.
 *
 * The corresponding C type is SVM_Boolean.
 *
 * \see variables
 */
SVM_TYPE typedef const void* SVM_Value_Boolean;

/** \typedef SVM_Value_Pointer
 * This type is used to represent a pointer the SVM can store in one of its memories.
 *
 * This type has no corresponding type in C, as it is made of:
 * - a memory address,
 * - a size.
 *
 * A pointer can then represent a zone in memory.
 *
 * \see variables
 */
SVM_TYPE typedef const void* SVM_Value_Pointer;

/** \typedef SVM_Value_Library
 * This type is used to represent a library the SVM can store in one of its memories.
 *
 * This type has no corresponding type in C, as it is made of an SVM_Code instance.
 *
 * \see variables
 */
SVM_TYPE typedef const void* SVM_Value_Library;

/** \typedef SVM_Value_Symbol
 * This type is used to represent a symbol the SVM can store in one of its memories.
 *
 * This type has no corresponding type in C, as it is made of:
 * - an SVM_Code instance,
 * - a local address.
 *
 * A symbol can be used in jumps or functions calls.
 *
 * \see variables
 */
SVM_TYPE typedef const void* SVM_Value_Symbol;

/** \typedef SVM_Value_PluginEntryPoint
 * This type is used to represent a plugin entry point the SVM can store in one of its memories.
 *
 * This type has no corresponding type in C, as it is made of:
 * - a plugin name,
 * - an entry name in this plugin.
 *
 * A plugin entry point can be used to reference an object in a plugin.
 *
 * \see variables
 */
SVM_TYPE typedef const void* SVM_Value_PluginEntryPoint;

/** \typedef SVM_Value_Interruption
 * This type is used to represent an interruption name the SVM can store in one of its memories.
 *
 * This type has no corresponding type in C, as it is made of:
 * - an internal interruption type,
 * - a plugin entry point for interruptions defined by plugins.
 *
 * An interruption name can be used to alter the program behavior in case of interruption, or can be used to interrupt a program, raising an event or an error.
 *
 * \see variables
 */
SVM_TYPE typedef const void* SVM_Value_Interruption;

/** \typedef SVM_Value_Plugin
 * This type is used to represent a value defined by a plugin the SVM can store in one of its memories.
 *
 * This type has no corresponding type in C, as it is made of:
 * - a plugin entry point describing the type of the value,
 * - a void* pointer managed by plugin callbacks.
 *
 * \see variables
 */
SVM_TYPE typedef const void* SVM_Value_Plugin;

/** \typedef SVM_Value_Automatic
 * This type is used to represent a null automatic value.
 *
 * This type has no corresponding type in C.
 *
 * \see variables
 */
SVM_TYPE typedef const void* SVM_Value_Automatic;

/** \} */

/** \defgroup configuration SVM plugin configuration
 * This module contains API functions used to configure a SVM plugin.
 *
 * The configuration of a plugin is done through a dedicated C callback function:
 * \code{.c}
 * void plugin_configure(void *plugin);
 * \endcode
 *
 * This function can perform some initialisations, but has to call the API function:
 * \code{.c}
 * void svm_plugin_configure(void *plugin_handler, const char *plugin_configuration, const char api_signature[], const char svm_version[]);
 * \endcode
 * to register all defined objects into the virtual machine.
 *
 * \see svm_plugin_configure
 * \{
 */

/**
 * This function helps the SVM to check the compatibility of a given plugin to its own API.
 * Any detected incompatibility aborts the boot of the SVM to prevent issues at run time.
 *
 * @returns A string containing a hash of the API, computed with the SVM_TYPE* and SVM_FUNCTION macros.
 */
SVM_FUNCTION const char* svm_api_get_signature();

/**
 * This function configures a plugin to the virtual machine.
 *
 * @param[in] plugin_handler An opaque pointer representing a plugin. This pointer is given by the SVM as the first parameter of the plugin_configure function.
 * @param[in] plugin_configuration A string representing the dependencies and the definitions of the plugin. Please refer to the SVM man page to have more details about this string syntax.
 * @param[in] api_signature A string representing the SVM API hash used at plugin compilation time. The macro SVM_API_SIGNATURE defined in the <svm/svm_compatibility.h> header should be used here.
 * @param[in] svm_version A string representing the SVM version used at plugin compilation time. The macro SVM_VERSION defined in the <svm/svm_compatibility.h> header should be used here.
 *
 * \note This function SHALL be called from the function
 * \code{.c}
 * void plugin_configure(void *plugin);
 * \endcode
 * defined once in each plugin to declare the plugin content to the SVM.
 * This specific plugin callback will be called at the virtual machine boot time to setup the full virtual machine architecture.
 *
 * A typical example of a plugin configuration function:
 * \code{.c}
 * #include <svm/svm_compatibility.h>
 *
 * ...
 *
 * void plugin_configure(void *plugin)
 * {
 *         svm_plugin_configure(plugin,
 *         "PLUGIN // put here the plugin name \n"
 *         "USE \n"
 *         ... // put here the plugin objects dependencies
 *         "DEFINE \n"
 *         ... // put here the objects the plugin promotes
 *         "",
 *         SVM_API_SIGNATURE,
 *         SVM_VERSION
 *         );
 * }
 * \endcode
 */
SVM_FUNCTION void svm_plugin_configure(void *plugin_handler, const char *plugin_configuration, const char api_signature[], const char svm_version[]);

/** \} */

/** \defgroup variables SVM variables
 * This module contains API functions to handle variables.
 *
 * A variable in this API refers to a generic pointer used as a key associated to an object internally managed by the virtual machine.
 *
 * There are two ways to obtain a variable:
 * - Instruction and function parameters are variables,
 * - API function return values are variables when the indicated type corresponds to a variable type.
 *
 * The scope of a variable can be:
 * - Limited to the plugin callback call (local variable): The variable will not be available in a subsequent plugin callback call,
 * - Extended to all plugin callback calls (global variable): The variable will be available in a subsequent plugin callback call within the same plugin.
 *
 * The scope of the object associated to a variable can be different when:
 * - Several variables refer to the same object,
 * - The object is used by the virtual machine outside the scope of variables.
 *
 * Variables are equally comparable: Two variables being equal refer to the same associated object:
 * \code{.c}
 * static SVM_Process process;
 *
 * SVM_Value instruction_one(const void *svm, SVM_Size argc, SVM_Parameter argv[])
 * {
 * 	process = svm_process_get_current(svm);
 * 	return NULL;
 * }
 *
 * SVM_Value instruction_two(const void *svm, SVM_Size argc, SVM_Parameter argv[])
 * {
 * 	if(svm_process_get_current(svm) == process)
 * 	{
 * 		// we get here only in the last process which invoked instruction one!
 * 	}
 * 	return NULL;
 * }
 * \endcode
 *
 * \see types
 * \{
 */

/**
 * This function creates an array of variables.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] size The size of the array.
 *
 * \returns A pointer to a C array of SVM_Variable. Every cell of the array has to be initialised by a value before being passed to an API function.
 *
 * \note The returned array is a valid SVM variable: its scope can be changed if needed, and the pointer shall not be freed explicitely.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Variable* svm_variable_array_new(const void *svm, SVM_Size size);

/**
 * This function alters the scope of the given variable.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] variable The variable to free.
 *
 * This variable becomes immediately invisible.
 *
 * The object associated to this variable may be deleted immediately.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION void svm_variable_delete(const void *svm, const SVM_Variable variable);

/** \defgroup scopevariables SVM variables scope
 * This module contains API functions to handle the scope of variables.
 * \{
 */

/**
 * This function alters the scope of the given variable.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] variable The variable to upgrade as global variable.
 *
 * This variable becomes visible in all further callbacks of the plugin, until either svm_variable_scope_set_local or svm_variable_delete is called on the same variable.
 *
 * The object associated to this variable is also kept alive when its scope is set global.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION void svm_variable_scope_set_global(const void *svm, const SVM_Variable variable);

/**
 * This function alters the scope of the given variable.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] variable The variable to downgrade as local variable.
 *
 * This variable becomes visible only in the current callback of the plugin, unless svm_variable_scope_set_global is called on the same variable before the end of the callback.
 *
 * The object associated to this variable may be deleted when the callback reaches its end.
 *
 * \note This scope is the default one. This allows automatic clean-up of all transient values created during a plugin callback call.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION void svm_variable_scope_set_local(const void *svm, const SVM_Variable variable);

/**
 * This function alters the scope of the given variable.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] variable The variable to upgrade as shared variable.
 *
 * When a variable is set as shared, svm_variable_scope_set_global and svm_variable_scope_set_local API function calls are no longer idempotents.
 * If svm_variable_scope_set_global is called several times on the same variable, it will require the same number of calls to svm_variable_scope_set_local to set the variable as local.
 *
 * This mechanism allows a variable to be shared by several C/C++ structures within a plugin.
 *
 * \note If the variable is already global when passed to shared, all previous calls to svm_variable_scope_set_global are counted as one unique call.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 *
 * \see svm_variable_scope_set_global
 * \see svm_variable_scope_set_local
 */
SVM_FUNCTION void svm_variable_scope_set_shared(const void *svm, const SVM_Variable variable);

/**
 * This function alters the scope of the given variable.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] variable The variable to downgrade as regular variable.
 *
 * When a variable is reset as regular, svm_variable_scope_set_global and svm_variable_scope_set_local API function calls become idempotents.
 * If svm_variable_scope_set_global is called several times on the same variable, only one call to svm_variable_scope_set_local will set the variable as local.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 *
 * \see svm_variable_scope_set_global
 * \see svm_variable_scope_set_local
 */
SVM_FUNCTION void svm_variable_scope_reset_shared(const void *svm, const SVM_Variable variable);

/**
 * This function checks whether a variable is shared.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] variable The variable to check.
 *
 * \returns TRUE when the variable is shared, FALSE otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 *
 * \see svm_variable_scope_set_shared
 * \see svm_variable_scope_reset_shared
 */
SVM_FUNCTION SVM_Boolean svm_variable_scope_is_shared(const void *svm, const SVM_Variable variable);

/**
 * This function checks whether a variable is local.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] variable The variable to check.
 *
 * \returns TRUE when the variable is local, FALSE otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 *
 * \see svm_variable_scope_set_shared
 * \see svm_variable_scope_set_global
 * \see svm_variable_scope_set_local
 * \see svm_variable_scope_is_global
 */
SVM_FUNCTION SVM_Boolean svm_variable_scope_is_local(const void *svm, const SVM_Variable variable);

/**
 * This function checks whether a variable is global.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] variable The variable to check.
 *
 * \returns TRUE when the variable is global, FALSE otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 *
 * \see svm_variable_scope_set_shared
 * \see svm_variable_scope_set_global
 * \see svm_variable_scope_set_local
 * \see svm_variable_scope_is_local
 */
SVM_FUNCTION SVM_Boolean svm_variable_scope_is_global(const void *svm, const SVM_Variable variable);

/** \} */

/** \defgroup detectvariables SVM variables detection
 * This module contains API functions to detect variable types.
 * \{
 */

/**
 * This function checks whether a variable is valid.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] variable The variable to diagnose.
 *
 * \returns TRUE if the variable is known by the Virtual Machine, FALSE otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Boolean svm_variable_is_valid(const void *svm, const SVM_Variable variable);

/**
 * This function checks whether a variable contains a value.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] variable The variable to diagnose.
 *
 * \returns TRUE if the variable can be used as a SVM_Value, FALSE otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Boolean svm_variable_type_is_value(const void *svm, const SVM_Variable variable);

/**
 * This function checks whether a variable contains a structure.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] variable The variable to diagnose.
 *
 * \returns TRUE if the variable can be used as a SVM_Structure, FALSE otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Boolean svm_variable_type_is_structure(const void *svm, const SVM_Variable variable);

/**
 * This function checks whether a variable contains a parameter.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] variable The variable to diagnose.
 *
 * \returns TRUE if the variable can be used as a SVM_Parameter, FALSE otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Boolean svm_variable_type_is_parameter(const void *svm, const SVM_Variable variable);

/**
 * This function checks whether a variable contains an access control.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] variable The variable to diagnose.
 *
 * \returns TRUE if the variable can be used as a SVM_AccessControl, FALSE otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Boolean svm_variable_type_is_accesscontrol(const void *svm, const SVM_Variable variable);

/**
 * This function checks whether a variable contains a memory zone.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] variable The variable to diagnose.
 *
 * \returns TRUE if the variable can be used as a SVM_Memory_Zone, FALSE otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Boolean svm_variable_type_is_memoryzone(const void *svm, const SVM_Variable variable);

/**
 * This function checks whether a variable contains a SVM code.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] variable The variable to diagnose.
 *
 * \returns TRUE if the variable can be used as a SVM_Code, FALSE otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Boolean svm_variable_type_is_code(const void *svm, const SVM_Variable variable);

/**
 * This function checks whether a variable contains a kernel.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] variable The variable to diagnose.
 *
 * \returns TRUE if the variable can be used as a SVM_Kernel, FALSE otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Boolean svm_variable_type_is_kernel(const void *svm, const SVM_Variable variable);

/**
 * This function checks whether a variable contains a process.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] variable The variable to diagnose.
 *
 * \returns TRUE if the variable can be used as a SVM_Process, FALSE otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Boolean svm_variable_type_is_process(const void *svm, const SVM_Variable variable);

/**
 * This function checks whether a variable contains a process lock.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] variable The variable to diagnose.
 *
 * \returns TRUE if the variable can be used as a SVM_Process_Lock, FALSE otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Boolean svm_variable_type_is_processlock(const void *svm, const SVM_Variable variable);

/**
 * This function checks whether a variable contains a scheduler.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] variable The variable to diagnose.
 *
 * \returns TRUE if the variable can be used as a SVM_Scheduler, FALSE otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Boolean svm_variable_type_is_scheduler(const void *svm, const SVM_Variable variable);

/**
 * This function checks whether a variable contains a lock.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] variable The variable to diagnose.
 *
 * \returns TRUE if the variable can be used as a SVM_Lock, FALSE otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Boolean svm_variable_type_is_lock(const void *svm, const SVM_Variable variable);

/**
 * This function checks whether a variable contains a lock guard in read mode.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] variable The variable to diagnose.
 *
 * \returns TRUE if the variable can be used as a SVM_LockGuard_Read, FALSE otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Boolean svm_variable_type_is_lockguardread(const void *svm, const SVM_Variable variable);

/**
 * This function checks whether a variable contains a lock guard in write mode.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] variable The variable to diagnose.
 *
 * \returns TRUE if the variable can be used as a SVM_LockGuard_Write, FALSE otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Boolean svm_variable_type_is_lockguardwrite(const void *svm, const SVM_Variable variable);

/**
 * This function checks whether a variable contains an event queue.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] variable The variable to diagnose.
 *
 * \returns TRUE if the variable can be used as a SVM_Event_Queue, FALSE otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Boolean svm_variable_type_is_eventqueue(const void *svm, const SVM_Variable variable);

/**
 * This function checks whether a variable contains an event queue address.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] variable The variable to diagnose.
 *
 * \returns TRUE if the variable can be used as a SVM_Event_Queue_Address, FALSE otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Boolean svm_variable_type_is_eventqueueaddress(const void *svm, const SVM_Variable variable);

/** \} */

/** \} */

/** \defgroup string SVM strings
 * This module contains API functions to handle SVM strings.
 *
 * Strings are structures used to exchange memory buffers between the virtual machine and the plugin callback functions.
 *
 * They can be managed by the virtual machine when created using API functions, but can also be manually managed by the
 * plugin code.
 *
 * \{
 */

/**
 * This function creates an instance of a SVM_String from a memory buffer.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] buffer The start of the buffer.
 * \param[in] size The size of the buffer.
 *
 * \returns The corresponding SVM_String. Its buffer is managed by the SVM like variables.
 *
 * \see variables
 */
SVM_FUNCTION SVM_String svm_string_new(const void *svm, const char *buffer, const SVM_Size size);

/**
 * This function creates an instance of a SVM_String from a C string.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] string A C-nul-terminated string to initiate the string.
 *
 * \returns The corresponding SVM_String. Its buffer is managed by the SVM like variables.
 *
 * \note This function adds an extra nul character at the end of the buffer, not counted in the size of the created string.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 *
 * \see variables
 */
SVM_FUNCTION SVM_String svm_string_new__raw(const void *svm, const char *string);

/**
 * This function creates an instance of a SVM_String uninitialised.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] size The size of the created string.
 *
 * \returns The corresponding SVM_String. Its buffer is managed by the SVM like variables.
 *
 * \see variables
 */
SVM_FUNCTION SVM_String svm_string_new__buffer(const void *svm, const SVM_Size size);

/**
 * This function creates an instance of a SVM_String from two SVM_String.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] first The start of the new string.
 * \param[in] second The end of the new string.
 *
 * \returns The joined SVM_String. Its buffer is managed by the SVM like variables.
 *
 * \see variables
 */
SVM_FUNCTION SVM_String svm_string_join(const void *svm, const SVM_String first, const SVM_String second);

/**
 * This function creates an instance of a SVM_String from a SVM_String and a C nul terminated string.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] first The start of the new string.
 * \param[in] second The end of the new string.
 *
 * \returns The joined SVM_String. Its buffer is managed by the SVM like variables.
 *
 * \see variables
 */
SVM_FUNCTION SVM_String svm_string_join__raw(const void *svm, const SVM_String first, const char *second);

/**
 * This function creates an instance of a SVM_String from a SVM_String and a memory buffer.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] first The start of the new string.
 * \param[in] second The end of the new string.
 * \param[in] size The size of the end of the new string.
 *
 * \returns The joined SVM_String. Its buffer is managed by the SVM like variables.
 *
 * \see variables
 */
SVM_FUNCTION SVM_String svm_string_join__buffer(const void *svm, const SVM_String first, const char *second, const SVM_Size size);

/**
 * This function compares two SVM_String.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] left The left operand of the equality.
 * \param[in] right The right operand of the equality.
 *
 * \returns TRUE if left and right contains the same string, FALSE otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Boolean svm_string_is_equal(const void *svm, const SVM_String left, const SVM_String right);

/**
 * This function compares a SVM_String to a C string.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] left The left operand of the equality.
 * \param[in] right The right operand of the equality.
 *
 * \returns TRUE if left and right contains the same string, FALSE otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Boolean svm_string_is_equal__raw(const void *svm, const SVM_String left, const char *right);

/**
 * This function compares a SVM_String to a C buffer.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] left The left operand of the equality.
 * \param[in] buffer The buffer, right operand of the equality.
 * \param[in] size The buffer size.
 *
 * \returns TRUE if left and right contains the same string, FALSE otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Boolean svm_string_is_equal__buffer(const void *svm, const SVM_String left, const char *buffer, const SVM_Size size);

/** \} */

/** \defgroup parameters SVM parameters
 * This module contains API functions to handle SVM parameters.
 *
 * Parameters are special variables used as interface to pass data in instruction and function calls:
 * - In instruction plugin callback functions, parameters are generated by the virtual machine from the arguments evaluated in the SVM code,
 * - In function plugin callback functions, parameters are generated by the plugin callback calls,
 * - In order to pass parameters to functions, an array of parameter has to be created and passed to the proper API function.
 *
 * \{
 */

/** \defgroup valueparameters SVM value parameters
 * This module contains API functions to handle SVM value parameters.
 * \{
 */

/**
 * This function checks whether a callback parameter is a value.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] parameter The callback parameter to check.
 *
 * \returns TRUE if the parameter is a value, FALSE otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Boolean svm_parameter_type_is_value(const void *svm, const SVM_Parameter parameter);

/**
 * This function converts a parameter into a value.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] parameter The callback parameter to convert.
 *
 * \returns The value variable contained in the parameter.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the parameter does not contain a value.
 *
 * \see svm_parameter_type_is_value
 */
SVM_FUNCTION SVM_Value svm_parameter_value_get(const void *svm, const SVM_Parameter parameter);

/**
 * This function creates a parameter from a value.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] value The value to be passed as a parameter.
 *
 * \returns The parameter containing the value.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Parameter svm_parameter_value_new(const void *svm, const SVM_Value value);

/** \} */

/** \defgroup markerparameters SVM marker parameters
 * This module contains API functions to handle SVM marker parameters.
 * \{
 */

/**
 * This function checks whether a parameter contains a marker.
 *
 * A marker is a symbol among = < <= << <> >> => > { } , ;
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] parameter The callback parameter to check.
 *
 * \returns TRUE if the parameter contains a marker, FALSE otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Boolean svm_parameter_type_is_marker(const void *svm, const SVM_Parameter parameter);

/**
 * This function converts a parameter into a marker.
 *
 * A marker is a symbol among = < <= << <> >> => > { } , ;
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] parameter The callback parameter to convert.
 *
 * \returns The marker as a SVM_String.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the parameter does not contain a marker.
 *
 * \see svm_parameter_type_is_marker
 */
SVM_FUNCTION SVM_String svm_parameter_marker_get(const void *svm, const SVM_Parameter parameter);

/**
 * This function creates a parameter from a marker string.
 *
 * A marker is a symbol among = < <= << <> >> => > { } , ;
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] marker The marker to be passed as a parameter.
 *
 * \returns A parameter containing the marker.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Parameter svm_parameter_marker_new(const void *svm, const SVM_String marker);

/**
 * This function creates a parameter from a marker string.
 *
 * A marker is a symbol among = < <= << <> >> => > { } , ;
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] marker The marker to be passed as a parameter.
 *
 * \returns A parameter containing the marker.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Parameter svm_parameter_marker_new__raw(const void *svm, const char *marker);

/**
 * This function looks up for a specific marker in a parameter array.
 *
 * A marker is a symbol among = < <= << <> >> => > { } , ;
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] argc The size of the parameter array.
 * \param[in] argv The parameter array.
 * \param[in] start The first index to search from.
 * \param[in] marker The marker to look up.
 *
 * \returns The index of the first found occurence, or argc when not found.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the start is outside parameter array valid indexes.
 * \throws FAILURE interruption when the marker is invalid.
 */
SVM_FUNCTION SVM_Index svm_parameter_marker_find(const void *svm, const SVM_Size argc, const SVM_Parameter argv[], const SVM_Index start, const SVM_String marker);

/**
 * This function looks up for a specific marker in a parameter array.
 *
 * A marker is a symbol among = < <= << <> >> => > { } , ;
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] argc The size of the parameter array.
 * \param[in] argv The parameter array.
 * \param[in] start The first index to search from.
 * \param[in] marker The marker to look up.
 *
 * \returns The index of the first found occurence, or argc when not found.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the start is outside parameter array valid indexes.
 * \throws FAILURE interruption when the marker is invalid.
 */
SVM_FUNCTION SVM_Index svm_parameter_marker_find__raw(const void *svm, const SVM_Size argc, const SVM_Parameter argv[], const SVM_Index start, const char *marker);

/**
 * This function looks up the next separator marker in a parameter array.
 *
 * A separator marker is a symbol among = <> , ;
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] argc The size of the parameter array.
 * \param[in] argv The parameter array.
 * \param[in] start The first index to search from.
 *
 * \returns The index of the first found occurence, or argc when not found.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the start is outside parameter array valid indexes.
 */
SVM_FUNCTION SVM_Index svm_parameter_marker_find_separator(const void *svm, const SVM_Size argc, const SVM_Parameter argv[], const SVM_Index start);

/**
 * This function looks up for the closing marker corresponding to an opening separator in a parameter array.
 *
 * A closing marker is a symbol among:
 * - > for < as opening marker,
 * - >> for << as opening marker,
 * - => for <= as opening marker,
 * - } for { as opening marker.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] argc The size of the parameter array.
 * \param[in] argv The parameter array.
 * \param[in] opening The index of the opening marker.
 *
 * The function will skip nested opening and closing markers.
 *
 * \returns The index of the closing marker occurence, or argc when not found.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the start is outside parameter array valid indexes.
 * \throws FAILURE interruption when the start is not an opening marker.
 */
SVM_FUNCTION SVM_Index svm_parameter_marker_find_closing(const void *svm, const SVM_Size argc, const SVM_Parameter argv[], const SVM_Index opening);

/** \} */

/** \defgroup keywordparameters SVM keyword parameters
 * This module contains API functions to handle SVM keyword parameters.
 * \{
 */

/**
 * This function checks whether a parameter contains a keyword.
 *
 * A keyword is a word in capital letters used as a flag or constant in parameter lists.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] parameter The callback parameter to check.
 *
 * \returns TRUE if the parameter contains a keyword, FALSE otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the marker string does not correspond to a valid marker.
 */
SVM_FUNCTION SVM_Boolean svm_parameter_type_is_keyword(const void *svm, const SVM_Parameter parameter);

/**
 * This function converts a parameter into a keyword.
 *
 * A keyword is a word in capital letters used as a flag or constant in parameter lists.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] parameter The callback parameter to convert.
 *
 * \returns The keyword as a SVM_String.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the parameter does not contain a keyword.
 *
 * \see svm_parameter_type_is_keyword
 */
SVM_FUNCTION SVM_String svm_parameter_keyword_get(const void *svm, const SVM_Parameter parameter);

/**
 * This function creates a parameter from a keyword string.
 *
 * A keyword is a word in capital letters used as a flag or constant in parameter lists.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] keyword The keyword to be passed as a parameter.
 *
 * \returns A parameter containing the keyword.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the keyword is empty or has a non capital-letter character.
 */
SVM_FUNCTION SVM_Parameter svm_parameter_keyword_new(const void *svm, const SVM_String keyword);

/**
 * This function creates a parameter from a keyword string.
 *
 * A keyword is a word in capital letters used as a flag or constant in parameter lists.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] keyword The keyword to be passed as a parameter.
 *
 * \returns A parameter containing the keyword.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the keyword is empty or has a non capital-letter character.
 */
SVM_FUNCTION SVM_Parameter svm_parameter_keyword_new__raw(const void *svm, const char *keyword);

/** \} */

/** \defgroup structureparameters SVM structure parameters
 * This module contains API functions to handle SVM structure parameters.
 *
 * \see structures
 * \{
 */

/**
 * This function checks whether a parameter contains a structure.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] parameter The callback parameter to check.
 *
 * \returns TRUE if the parameter contains a structure, FALSE otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Boolean svm_parameter_type_is_structure(const void *svm, const SVM_Parameter parameter);

/**
 * This function converts a parameter into a structure.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] parameter The callback parameter to check.
 *
 * \returns The structure as a SVM_Structure.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the parameter does not contain a structure.
 *
 * \see svm_parameter_type_is_structure
 * \see svm_structure_get_internal
 */
SVM_FUNCTION SVM_Structure svm_parameter_structure_get(const void *svm, const SVM_Parameter parameter);

/**
 * This function creates a parameter from a structure.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] structure The structure to be passed as a parameter.
 *
 * \returns A parameter containing the structure.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 *
 * \see svm_structure_new
 * \see svm_structure_new_null
 */
SVM_FUNCTION SVM_Parameter svm_parameter_structure_new(const void *svm, const SVM_Structure structure);

/** \} */

/** \defgroup variableparameters SVM variable parameters
 * This module contains API functions to handle SVM variable parameters.
 * \{
 */

/**
 * This function checks whether a parameter contains a variable.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] parameter The callback parameter to check.
 *
 * \returns TRUE if the parameter contains a variable, FALSE otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Boolean svm_parameter_type_is_variable(const void *svm, const SVM_Parameter parameter);

/**
 * This function converts a parameter into a variable.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] parameter The callback parameter to check.
 *
 * \returns The variable as a SVM_Variable.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the parameter does not contain a variable.
 *
 * \see svm_parameter_type_is_variable
 */
SVM_FUNCTION SVM_Variable svm_parameter_variable_get(const void *svm, const SVM_Parameter parameter);

/**
 * This function creates a parameter from a variable.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] variable The variable to be passed as a parameter.
 *
 * \returns A parameter containing the variable.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Parameter svm_parameter_variable_new(const void *svm, const SVM_Variable variable);

/** \} */

/**
 * This function creates an array of parameters.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] argc The size of the array.
 *
 * \returns A pointer to a C array of SVM_Parameter. Every cell of the array has to be initialised by a parameter before being passed to an API function.
 *
 * \note The returned array is a valid SVM variable: its scope can be changed if needed, and the pointer shall not be freed explicitely.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Parameter* svm_parameter_array_new(const void *svm, const SVM_Size argc);

/** \} */

/** \defgroup machine SVM machine
 * This module contains API functions to handle the SVM machine.
 *
 * \{
 */

/**
 * This function shuts down the whole machine, with a return code as an integer value.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] return_code The return code of the virtual machine.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws SECURITY interruption when this API function is called outside a system instruction.
 */
SVM_FUNCTION void svm_machine_shutdown(const void *svm, const SVM_Value_Integer return_code);

/**
 * This function shuts down the whole machine, with a return code as a raw integer.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] return_code The return code of the virtual machine.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws SECURITY interruption when this API function is called outside a system instruction.
 */
SVM_FUNCTION void svm_machine_shutdown__raw(const void *svm, const unsigned long int return_code);

/**
 * This function emits an error trace on the logger from a string value.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] trace The trace to emit.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION void svm_machine_trace(const void *svm, const SVM_Value_String trace);

/**
 * This function emits an error trace on the logger from a string.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] trace The trace to emit.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION void svm_machine_trace__string(const void *svm, const SVM_String trace);

/**
 * This function emits an error trace on the logger from a raw C string.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] trace The trace to emit.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION void svm_machine_trace__raw(const void *svm, const char *trace);

/**
 * This function returns the list of available interruptions.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 *
 * Internal and external interruptions are returned.
 *
 * \returns A pointer to a C array of SVM_Value_Interruption, terminated by a C/C++ null pointer.
 *
 * \note The returned array is a valid SVM variable: its scope can be changed if needed, and the pointer shall not be freed explicitely.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 *
 */
SVM_FUNCTION SVM_Value_Interruption* svm_machine_list_interruption(const void *svm);

/**
 * This function returns a value from the virtual machine installation.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] key The key of the configuration to get.
 *
 * \returns A string containing the value when the key is defined.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the key does not exist.
 *
 */
SVM_FUNCTION SVM_String svm_machine_config(const void *svm, const char *key);

/**
 * This function indicates when the virtual machine is executed in debug mode.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 *
 * \returns A boolean set to TRUE when the debugger is activated, and FALSE otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 *
 */
SVM_FUNCTION SVM_Boolean svm_machine_mode_is_debug(const void *svm);

/**
 * This function indicates when the virtual machine is executed in performance mode.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 *
 * \returns A boolean set to TRUE when the profiler is activated, and FALSE otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 *
 */
SVM_FUNCTION SVM_Boolean svm_machine_mode_is_performance(const void *svm);

/** \} */

/** \defgroup plugins SVM plugins
 * This module contains API functions to handle SVM plugins.
 *
 * Plugins are not directly accessible in the present API, but some detection functions are defined to allow dynamic detection of plugin dependencies.
 *
 * \{
 */

/**
 * This function extracts a string representation of all defined objects in plugins.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 *
 * \returns The string representation of all objects defined in plugins.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_String svm_plugin_print(const void *svm);

/** \defgroup pluginobjects SVM plugin objects
 * This module contains API functions to handle SVM plugin objects.
 *
 * \{
 */

/**
 * This function checks whether a call to an instruction is possible.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] instruction The instruction name.
 * \param[in] argc The number of parameters.
 * \param[in] argv A sample of parameters. Only their types are used.
 * \param[in] result A sample of the result of the function.
 * The result parameter can contain:
 * - an initialised value to simulate a plain result return,
 * - a null value to simulate the possibility to return a null value,
 * - a NULL pointer to simulate an instruction returning no value at all.
 *
 * \returns TRUE if the call is compatible with the instruction definition, FALSE otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Boolean svm_plugin_has_instruction(const void *svm, const SVM_Value_PluginEntryPoint instruction, const SVM_Size argc, const SVM_Parameter argv[], SVM_Parameter result);

/**
 * This function checks whether a type exists.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] type The type name to check.
 *
 * \returns TRUE if the type exists, FALSE otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Boolean svm_plugin_has_type(const void *svm, const SVM_Value_PluginEntryPoint type);

/**
 * This function checks whether a call to a function is possible.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] function The function name.
 * \param[in] argc The number of parameters.
 * \param[in] argv A sample of parameters. Only their types are used.
 * \param[in] result A sample of the result of the function.
 * The result parameter can contain:
 * - an initialised value or structure to simulate a plain result return,
 * - a null value to simulate the possibility to return a null value,
 * - a NULL pointer to simulate an instruction returning no value at all.
 *
 * \returns TRUE if the call is compatible with the function definition, FALSE otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Boolean svm_plugin_has_function(const void *svm, const SVM_Value_PluginEntryPoint function, const SVM_Size argc, const SVM_Parameter argv[], SVM_Parameter result);

/**
 * This function checks whether a structure exists.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] structure The structure name to check.
 *
 * \returns TRUE if the structure exists, FALSE otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Boolean svm_plugin_has_structure(const void *svm, const SVM_Value_PluginEntryPoint structure);

/**
 * This function checks whether an interruption exists.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] interruption The interruption name to check.
 *
 * \returns TRUE if the interruption exists, FALSE otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Boolean svm_plugin_has_interruption(const void *svm, const SVM_Value_PluginEntryPoint interruption);

/**
 * This function checks whether a sequencer exists.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] sequencer The sequencer name to check.
 *
 * \returns TRUE if the sequencer exists, FALSE otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Boolean svm_plugin_has_sequencer(const void *svm, const SVM_Value_PluginEntryPoint sequencer);

/**
 * This function checks whether a scheduler exists.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] scheduler The scheduler name to check.
 *
 * \returns TRUE if the scheduler exists, FALSE otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Boolean svm_plugin_has_scheduler(const void *svm, const SVM_Value_PluginEntryPoint scheduler);

/** \} */

/** \defgroup pluginconfiguration SVM plugin configuration
 * This module contains API functions to handle SVM plugin configuration.
 *
 * These functions allow plugin code to check and retrieve values associated to the options and arguments passed at plugin invokation.
 *
 * \{
 */

/**
 * This function checks whether an option exists.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] option The option name to check.
 *
 * \returns TRUE if the option exists, FALSE otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Boolean svm_plugin_has_option(const void *svm, const SVM_Value_PluginEntryPoint option);

/**
 * This function checks whether an argument exists.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] argument The argument name to check.
 *
 * \returns TRUE if the argument exists, FALSE otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Boolean svm_plugin_has_argument(const void *svm, const SVM_Value_PluginEntryPoint argument);

/**
 * This function checks whether an extra argument array exists.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] arguments The extra argument array name to check.
 *
 * \returns TRUE if the extra argument array exists, FALSE otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Boolean svm_plugin_has_arguments(const void *svm, const SVM_Value_PluginEntryPoint arguments);

/**
 * This function returns the value associated to an option.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] option The option name of the value to retrieve.
 *
 * \returns The value associated to the option.
 * 
 * \note The value is constant, can be null and can have a type among integer, string and boolean.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the option does not exist.
 *
 * \see svm_plugin_has_option
 */
SVM_FUNCTION SVM_Value svm_plugin_get_option(const void *svm, const SVM_Value_PluginEntryPoint option);

/**
 * This function returns the value associated to an argument.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] argument The argument name of the value to retrieve.
 *
 * \returns The value associated to the argument.
 * 
 * \note The value is constant and can have a type among integer and string.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the argument does not exist.
 *
 * \see svm_plugin_has_argument
 */
SVM_FUNCTION SVM_Value svm_plugin_get_argument(const void *svm, const SVM_Value_PluginEntryPoint argument);

/**
 * This function returns the values array associated to an extra arguments array.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] arguments The extra arguments array name of the value to retrieve.
 *
 * \returns The values array associated to the extra arguments array, terminated by a C null pointer.
 * 
 * \note The values are constant and can have a type among integer and string.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the extra arguments array does not exist.
 *
 * \see svm_plugin_has_arguments
 */
SVM_FUNCTION SVM_Value* svm_plugin_get_arguments(const void *svm, const SVM_Value_PluginEntryPoint arguments);

/** \} */

/** \} */

/** \defgroup structures SVM structures
 * This module contains API functions to handle SVM structures.
 *
 * Structures are special values used solely in plugin callback functions.
 *
 * Their behavior is similar to plugin defined types, and can be used as plugin function parameters
 *
 * \see functions
 * \see structureparameters
 *
 * \{
 */

/**
 * This function creates a new structure handler to convey a raw structure pointer.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] type The structure type name.
 * \param[in] value The structure raw pointer.
 * 
 * \returns The structure handler containing the raw pointer.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Structure svm_structure_new(const void *svm, const SVM_Value_PluginEntryPoint type, void *value);

/**
 * This function creates a new null structure handler.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] type The structure type name.
 * 
 * \returns The structure handler without value.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Structure svm_structure_new_null(const void *svm, const SVM_Value_PluginEntryPoint type);

/**
 * This function retrieves the type name of a structure.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] structure The structure to be diagnosed.
 *
 * \returns The type name of the structure.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Value_PluginEntryPoint svm_structure_get_type(const void *svm, const SVM_Structure structure);

/**
 * This function retrieves the raw pointer of a structure.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] type The expected type name of the structure.
 * \param[in] structure The structure to be diagnosed.
 *
 * \returns The raw pointer contained by the structure.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the structure type name mismatches the expected one.
 *
 * \see svm_value_pluginentrypoint_new__raw
 */
SVM_FUNCTION void* svm_structure_get_internal(const void *svm, const SVM_Value_PluginEntryPoint type, const SVM_Structure structure);

/**
 * This function copies a structure.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] structure The structure to be copied.
 *
 * \returns The copied structure.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the structure type does not support the copy operation.
 */
SVM_FUNCTION SVM_Structure svm_structure_copy(const void *svm, const SVM_Structure structure);

/** \} */

/** \defgroup functions SVM functions
 * This module contains API functions to handle SVM functions.
 *
 * Functions are a generic mechanism to allow plugin cooperation:
 * - A plugin can trigger a call to another plugin callback function,
 * - Type checking is done like for instruction calls,
 * - The caller plugin callback function can use the name and parameters from its own parameters to allow genericity.
 *
 * \{
 */

/**
 * This function calls a plugin defined function.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] name The plugin function name to call.
 * \param[in] argc The number of parameters.
 * \param[in] argv The function parameters. These parameters can be:
 *  - values,
 *  - markers,
 *  - keywords,
 *  - structures.
 *
 * \note The called function is not a raw plugin callback function.
 * Callable functions are objects defined as FUNCTION in plugins.
 *
 * \note The called function will not share the caller SVM first pointer.
 * The current kernel, process or scheduler remain the same in the called function when the call is done within a kernel.
 * But all local variables are not accessible to the function, and the global variables are the ones of the plugin of the called function.
 *
 * \note If a call to an instruction callback has to performed from a plugin, a function can be created in the plugin code containing the instruction callback to delegate the call:
 * \code{.c}
 * SVM_Variable function_<instruction name>(const void *svm, const unsigned long int argc, const SVM_Parameter argv[])
 * {
 * 	return instruction_<instruction name>(svm, argc, argv);
 * }
 * \endcode
 * This allows the plugin developper to allow only a subset of instructions to be called from other plugins.
 *
 * \warning If the argv size is smaller than argc, it will result in an undefined behavior.
 *
 * Usage example:
 * \code{.c}
 * SVM_Parameter *parameters = svm_parameter_array_new(svm,3);
 * parameters[0] = svm_parameter_value_new(svm,svm_parameter_value_get(svm,argv[0]));
 * parameters[1] = svm_parameter_marker_new__raw(svm,">");
 * parameters[2] = svm_parameter_value_new(svm,svm_value_integer_new(svm,42));
 * // call to FUNCTION remote.compare . ( [ < > = ] INT | 'IN' { INT , INT } ) -> BLN
 * SVM_Value_Boolean result = svm_function_call(svm,svm_value_pluginentrypoint_new__raw(svm,"remote","compare"),3,parameters);
 * if(svm_value_boolean_get(svm,result))
 * {
 * 	// ...
 * }
 * \endcode
 *
 * \returns The function result. This result can be:
 *  - a value,
 *  - a null value,
 *  - a structure,
 *  - a variable,
 *  - the NULL pointer when no result is returned.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the function does not exist.
 * \throws FAILURE interruption when the parameters function are not compatible with the function definition.
 * \throws FAILURE interruption when the return value of the function is not compatible with the function definition.
 *
 * \see svm_parameter_array_new
 */
SVM_FUNCTION SVM_Variable svm_function_call(const void *svm, const SVM_Value_PluginEntryPoint name, const SVM_Size argc, SVM_Parameter argv[]);

/** \} */

/** \defgroup types SVM types
 * This module contains API functions to handle SVM types.
 *
 * Types are a main characteristic of values, indicating which operations can be applied to values.
 *
 * \see values
 *
 * \{
 */

/**
 * This function creates an internal value type.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] internal The internal type name.
 *
 * \returns The type containing the internal type.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Type svm_type_new_internal(const void *svm, const SVM_Type_Internal internal);

/**
 * This function creates an external value type.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] external The external type name.
 *
 * \returns The type containing the external type.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Type svm_type_new_external(const void *svm, const SVM_Value_PluginEntryPoint external);

/**
 * This function indicates whether a type contains an internal value type.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] type The type to check.
 *
 * \returns TRUE if the type contains an internal type, FALSE otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Boolean svm_type_is_internal(const void *svm, const SVM_Type type);

/**
 * This function indicates whether a type contains an external value type.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] type The type to check.
 *
 * \returns TRUE if the type contains an external type, FALSE otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Boolean svm_type_is_external(const void *svm, const SVM_Type type);

/**
 * This function retrieves the internal type from a type.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] type The type to get.
 *
 * \returns The internal type.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the type is not an internal type.
 */
SVM_FUNCTION SVM_Type_Internal svm_type_get_internal(const void *svm, const SVM_Type type);

/**
 * This function retrieves the external type from a type.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] type The type to get.
 *
 * \returns The external type.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the type is not an external type.
 */
SVM_FUNCTION SVM_Value_PluginEntryPoint svm_type_get_external(const void *svm, const SVM_Type type);

/**
 * This function creates a copy of a type.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] type The type to copy.
 *
 * \returns The copy of the type.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Type svm_type_copy(const void *svm, const SVM_Type type);

/**
 * This function renders a type as a string.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] type The type to print.
 *
 * \returns The string representation of the type.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_String svm_type_print(const void *svm, const SVM_Type type);

/**
 * This function checks whether a type is a specific internal type.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] type The type to check.
 * \param[in] internal The internal type to check to.
 *
 * \returns TRUE when the type is the given internal type, and FALSE otherwise.
 *
 * \note When the type is an external type, FALSE is returned.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Boolean svm_type_equal_internal(const void *svm, const SVM_Type type, const SVM_Type_Internal internal);

/**
 * This function checks whether a type is a specific external type.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] type The type to check.
 * \param[in] external The external type to check to.
 *
 * \returns TRUE when the type is the given external type, and FALSE otherwise.
 *
 * \note When the type is an internal type, FALSE is returned.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Boolean svm_type_equal_external(const void *svm, const SVM_Type type, const SVM_Value_PluginEntryPoint external);

/**
 * This function compares two types.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] left The left operand of the equality.
 * \param[in] right The right operand of the equality.
 *
 * \returns The comparison status of the two values.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 *
 * \see SVM_Comparison_Result
 */
SVM_FUNCTION SVM_Comparison_Result svm_type_compare(const void *svm, const SVM_Type left, const SVM_Type right);

/** \} */

/** \defgroup values SVM values
 * This module contains API functions to handle SVM values.
 *
 * Values are containers for data a memory can store.
 *
 * \note In parameters, values are always passed by reference.
 * Using a set API function on a value passed on a parameter containing a value read from a memory will change the value directly in the memory.
 * This can be used to implement an "extra return parameter" or use a value in memory as a state between several plugin callback function calls.
 *
 * \warning Changing a value written in memory will bypass all eventual memory synchronisation mecanisms. Use this feature with caution.
 *
 * \see memories
 * \see synchronisationmemory
 *
 * \{
 */

/** \defgroup integervalues SVM integer values
 * This module contains API functions to handle SVM integer values.
 * \{
 */

/**
 * This function creates an integer value the SVM can store in its memories from a raw integer.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] integer The integer to initiate the integer value.
 *
 * \returns The integer value.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Value_Integer svm_value_integer_new(const void *svm, const long int integer);

/**
 * This function creates an integer value the SVM can store in its memories without a value.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 *
 * \returns A non initialised integer value.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Value_Integer svm_value_integer_new_null(const void *svm);

/**
 * This function checks whether a value contains an integer.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] value The value to be diagnosed.
 *
 * \returns TRUE if the value contains an integer, FALSE otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Boolean svm_value_type_is_integer(const void *svm, const SVM_Value value);

/**
 * This function extracts the raw integer contained in an integer value.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] integer The integer value to extract from.
 *
 * \returns The contained integer.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the integer parameter does not contain an integer.
 *
 * \see svm_value_type_is_integer
 * \see svm_value_type_get
 */
SVM_FUNCTION long long int svm_value_integer_get(const void *svm, const SVM_Value_Integer integer);

/**
 * This function replaces the raw integer contained in an integer value from a raw integer.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in,out] value The integer value to update.
 * \param[in] integer The new integer value.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the value parameter does not contain an integer.
 * \throws FAILURE interruption when the value is a constant.
 *
 * \see svm_value_type_is_integer
 * \see svm_value_type_get
 */
SVM_FUNCTION void svm_value_integer_set(const void *svm, SVM_Value_Integer value, const long long int integer);

/** \} */

/** \defgroup stringvalues SVM string values
 * This module contains API functions to handle SVM string values.
 * \{
 */

/**
 * This function creates a string value the SVM can store in its memories from a SVM_String.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] string The raw string to initiate the string value.
 *
 * \returns The string value.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Value_String svm_value_string_new(const void *svm, const SVM_String string);

/**
 * This function creates a string value the SVM can store in its memories from a C nul-terminated string.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] string The raw string to initiate the string value.
 *
 * \returns The string value.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Value_String svm_value_string_new__raw(const void *svm, const char *string);

/**
 * This function creates a string value the SVM can store in its memories from a memory buffer.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] string The buffer to initiate the string value.
 * \param[in] size The buffer size.
 *
 * \warning The allocated buffer has to be large enough to contain at least size bytes, or it will result in an undefined behavior.
 *
 * \returns The string value.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Value_String svm_value_string_new__buffer(const void *svm, const char *string, const SVM_Size size);

/**
 * This function creates a string value the SVM can store in its memories without a value.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 *
 * \returns A non initialised string value.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Value_String svm_value_string_new_null(const void *svm);

/**
 * This function checks whether a value contains a string.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] value The value to be diagnosed.
 *
 * \returns TRUE if the value contains a string, FALSE otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Boolean svm_value_type_is_string(const void *svm, const SVM_Value value);

/**
 * This function extracts the SVM_String contained in a string value.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] string The string value to extract from.
 *
 * \returns The contained string.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the string parameter does not contain a string.
 *
 * \see svm_value_type_is_string
 * \see svm_value_type_get
 */
SVM_FUNCTION SVM_String svm_value_string_get(const void *svm, const SVM_Value_String string);

/**
 * This function replaces the raw string contained in a string value from a SVM_String.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in,out] value The string value to update.
 * \param[in] string The new string value.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the value parameter does not contain a string.
 * \throws FAILURE interruption when the value is a constant.
 *
 * \see svm_value_type_is_string
 * \see svm_value_type_get
 */
SVM_FUNCTION void svm_value_string_set(const void *svm, SVM_Value_String value, const SVM_String string);

/**
 * This function replaces the raw string contained in a string value from a C null-terminated string.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in,out] value The string value to update.
 * \param[in] string The new string value.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the value parameter does not contain a string.
 * \throws FAILURE interruption when the value is a constant.
 */
SVM_FUNCTION void svm_value_string_set__raw(const void *svm, SVM_Value_String value, const char *string);

/**
 * This function replaces the raw string contained in a string value from a buffer.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in,out] value The string value to update.
 * \param[in] string The new string buffer value.
 * \param[in] size The size of the new string buffer.
 *
 * \warning The allocated buffer has to be large enough to contain at least size bytes, or it will result in an undefined behavior.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the value parameter does not contain a string.
 * \throws FAILURE interruption when the value is a constant.
 */
SVM_FUNCTION void svm_value_string_set__buffer(const void *svm, SVM_Value_String value, const char *string, const SVM_Size size);

/** \} */

/** \defgroup booleanvalues SVM boolean values
 * This module contains API functions to handle SVM boolean values.
 * \{
 */

/**
 * This function creates a boolean value the SVM can store in its memories from a raw boolean.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] boolean The boolean to initiate the boolean value.
 *
 * \returns The boolean value.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Value_Boolean svm_value_boolean_new(const void *svm, const SVM_Boolean boolean);

/**
 * This function creates a boolean value the SVM can store in its memories from a C boolean.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] boolean The boolean to initiate the boolean value.
 *
 * \returns The boolean value.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Value_Boolean svm_value_boolean_new__raw(const void *svm, const int boolean);

/**
 * This function creates a boolean value the SVM can store in its memories without a value.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 *
 * \returns A non initialised boolean value.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Value_Boolean svm_value_boolean_new_null(const void *svm);

/**
 * This function checks whether a value contains a boolean.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] value The value to be diagnosed.
 *
 * \returns TRUE if the value contains a boolean, FALSE otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Boolean svm_value_type_is_boolean(const void *svm, const SVM_Value value);

/**
 * This function extracts the SVM_Boolean contained in a boolean value.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] boolean The boolean value to extract from.
 *
 * \returns The raw boolean contained in the value.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the boolean parameter does not contain an boolean.
 *
 * \see svm_value_type_is_boolean
 * \see svm_value_type_get
 */
SVM_FUNCTION SVM_Boolean svm_value_boolean_get(const void *svm, const SVM_Value_Boolean boolean);

/**
 * This function replaces the raw boolean contained in a boolean value from a SVM_Boolean.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in,out] value The boolean value to update.
 * \param[in] boolean The new boolean value.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the boolean parameter does not contain an boolean.
 * \throws FAILURE interruption when the value is a constant.
 *
 * \see svm_value_type_is_boolean
 * \see svm_value_type_get
 */
SVM_FUNCTION void svm_value_boolean_set(const void *svm, SVM_Value_Boolean value, const SVM_Boolean boolean);

/**
 * This function replaces the raw boolean contained in a boolean value from a C boolean.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] value The boolean value to update.
 * \param[in] boolean The new boolean value.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the value parameter does not contain an boolean.
 * \throws FAILURE interruption when the value is a constant.
 *
 * \see svm_value_type_is_boolean
 * \see svm_value_type_get
 */
SVM_FUNCTION void svm_value_boolean_set__raw(const void *svm, SVM_Value_Boolean value, const int boolean);

/** \} */

/** \defgroup pointervalues SVM pointer values
 * This module contains API functions to handle SVM pointer values.
 * \{
 */

/**
 * This function creates a pointer value the SVM can store in its memories from two integer values.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] address The first address of the pointer.
 * \param[in] size The size of the pointer.
 * 
 * \returns The pointer value.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the address or the size is a negative number.
 */
SVM_FUNCTION SVM_Value_Pointer svm_value_pointer_new(const void *svm, const SVM_Value_Integer address, const SVM_Value_Integer size);

/**
 * This function creates a pointer value the SVM can store in its memories from two raw integers.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] address The first address of the pointer.
 * \param[in] size The size of the pointer.
 * 
 * \returns The pointer value.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Value_Pointer svm_value_pointer_new__raw(const void *svm, const SVM_Address address, const SVM_Size size);

/**
 * This function creates a pointer value the SVM can store in its memories without a value.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 *
 * \returns A non initialised pointer value.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Value_Pointer svm_value_pointer_new_null(const void *svm);

/**
 * This function checks whether a value contains a pointer.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] value The value to be diagnosed.
 *
 * \returns TRUE if the value contains a pointer, FALSE otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Boolean svm_value_type_is_pointer(const void *svm, const SVM_Value value);

/**
 * This function extracts the first pointer address contained in a pointer value.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] pointer The pointer value to extract from.
 *
 * \returns The first address of the pointer.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the pointer parameter does not contain an pointer.
 *
 * \see svm_value_type_is_pointer
 * \see svm_value_type_get
 */
SVM_FUNCTION SVM_Address svm_value_pointer_get_address(const void *svm, const SVM_Value_Pointer pointer);

/**
 * This function extracts the pointer size contained in a pointer value.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] pointer The pointer value to extract from.
 *
 * \returns The size of the pointer.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the pointer parameter does not contain an pointer.
 *
 * \see svm_value_type_is_pointer
 * \see svm_value_type_get
 */
SVM_FUNCTION SVM_Size svm_value_pointer_get_size(const void *svm, const SVM_Value_Pointer pointer);

/**
 * This function replaces the pointer value by another pointer value.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in,out] value The pointer value to update.
 * \param[in] pointer The pointer to use for update.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the value parameter does not contain an pointer.
 * \throws FAILURE interruption when the pointer parameter does not contain an pointer.
 * \throws FAILURE interruption when the value is a constant.
 *
 * \see svm_value_type_is_pointer
 * \see svm_value_type_get
 */
SVM_FUNCTION void svm_value_pointer_set(const void *svm, SVM_Value_Pointer value, const SVM_Value_Pointer pointer);

/**
 * This function replaces the first address contained in a pointer value from an integer value.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in,out] value The pointer value to update.
 * \param[in] address The new first address of the pointer.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the value parameter does not contain an pointer.
 * \throws FAILURE interruption when the address or the size is a negative integer.
 * \throws FAILURE interruption when the value is a constant.
 *
 * \see svm_value_type_is_pointer
 * \see svm_value_type_get
 */
SVM_FUNCTION void svm_value_pointer_set_address(const void *svm, SVM_Value_Pointer value, const SVM_Value_Integer address);

/**
 * This function replaces the first address contained in a pointer value from a raw value.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in,out] value The pointer value to update.
 * \param[in] address The new first address of the pointer.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the pointer parameter does not contain an pointer.
 * \throws FAILURE interruption when the value is a constant.
 *
 * \see svm_value_type_is_pointer
 * \see svm_value_type_get
 */
SVM_FUNCTION void svm_value_pointer_set_address__raw(const void *svm, SVM_Value_Pointer value, const SVM_Address address);

/**
 * This function replaces the size contained in a pointer value from an integer value.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in,out] value The pointer value to update.
 * \param[in] size The new pointer size.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the value parameter does not contain an pointer.
 * \throws FAILURE interruption when the address or the size is a negative integer.
 * \throws FAILURE interruption when the value is a constant.
 *
 * \see svm_value_type_is_pointer
 * \see svm_value_type_get
 */
SVM_FUNCTION void svm_value_pointer_set_size(const void *svm, SVM_Value_Pointer value, const SVM_Value_Integer size);

/**
 * This function replaces the size contained in a pointer value from a raw value.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in,out] value The pointer value to update.
 * \param[in] size The new pointer size.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the pointer parameter does not contain an pointer.
 * \throws FAILURE interruption when the value is a constant.
 *
 * \see svm_value_type_is_pointer
 * \see svm_value_type_get
 */
SVM_FUNCTION void svm_value_pointer_set_size__raw(const void *svm, SVM_Value_Pointer value, const SVM_Size size);

/**
 * This function replaces the first address and the size contained in a pointer value from two integer values.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in,out] value The pointer value to update.
 * \param[in] address The new first address of the pointer.
 * \param[in] size The new pointer size.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the value parameter does not contain an pointer.
 * \throws FAILURE interruption when the address or the size is a negative integer.
 * \throws FAILURE interruption when the value is a constant.
 *
 * \see svm_value_type_is_pointer
 * \see svm_value_type_get
 */
SVM_FUNCTION void svm_value_pointer_set_addresssize(const void *svm, SVM_Value_Pointer value, const SVM_Value_Integer address, const SVM_Value_Integer size);

/**
 * This function replaces the first address and the size contained in a pointer value from two raw values.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in,out] value The pointer value to update.
 * \param[in] address The new first address of the pointer.
 * \param[in] size The new pointer size.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the pointer parameter does not contain an pointer.
 * \throws FAILURE interruption when the value is a constant.
 *
 * \see svm_value_type_is_pointer
 * \see svm_value_type_get
 */
SVM_FUNCTION void svm_value_pointer_set_addresssize__raw(const void *svm, SVM_Value_Pointer value, const SVM_Address address, const SVM_Size size);

/** \} */

/** \defgroup libraryvalues SVM library values
 * This module contains API functions to handle SVM library values.
 * \{
 */

/**
 * This function creates a library value the SVM can store in its memories from a SVM_Code.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] code The code of the library.
 *
 * \returns The library value.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Value_Library svm_value_library_new(const void *svm, const SVM_Code code);

/**
 * This function creates a library value the SVM can store in its memories without a value.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 *
 * \returns A non initialised library value.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Value_Library svm_value_library_new_null(const void *svm);

/**
 * This function checks whether a value contains a library.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] value The value to be diagnosed.
 *
 * \returns TRUE if the value contains a library, FALSE otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Boolean svm_value_type_is_library(const void *svm, const SVM_Value value);

/**
 * This function extracts the code contained in a library value.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] library The library value to extract from.
 *
 * \returns The code contained in the value.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the library parameter does not contain an library.
 *
 * \see svm_value_type_is_library
 * \see svm_value_type_get
 */
SVM_FUNCTION SVM_Code svm_value_library_get_code(const void *svm, const SVM_Value_Library library);

/**
 * This function replaces the library value by another library value.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in,out] value The library value to update.
 * \param[in] library The library to use for update.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the value parameter does not contain an library.
 * \throws FAILURE interruption when the library parameter does not contain an library.
 * \throws FAILURE interruption when the value is a constant.
 *
 * \see svm_value_type_is_library
 * \see svm_value_type_get
 */
SVM_FUNCTION void svm_value_library_set(const void *svm, SVM_Value_Library value, const SVM_Value_Library library);

/**
 * This function replaces the code contained in a library value from a SVM_Code.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in,out] value The library value to update.
 * \param[in] code The new code value.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the value parameter does not contain an library.
 * \throws FAILURE interruption when the value is a constant.
 *
 * \see svm_value_type_is_library
 * \see svm_value_type_get
 */
SVM_FUNCTION void svm_value_library_set_code(const void *svm, SVM_Value_Library value, const SVM_Code code);

/** \} */

/** \defgroup symbolvalues SVM symbol values
 * This module contains API functions to handle SVM symbol values.
 * \{
 */

/**
 * This function creates a symbol value the SVM can store in its memories from a SVM code and an address.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] code The code of the library.
 * \param[in] symbol The address into the library.
 *
 * \returns The symbol value.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Value_Symbol svm_value_symbol_new(const void *svm, const SVM_Code code, const SVM_Address symbol);

/**
 * This function creates a symbol value the SVM can store in its memories without a value.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 *
 * \returns A non initialised symbol value.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Value_Symbol svm_value_symbol_new_null(const void *svm);

/**
 * This function checks whether a value contains a symbol.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] value The value to be diagnosed.
 *
 * \returns TRUE if the value contains a symbol, FALSE otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Boolean svm_value_type_is_symbol(const void *svm, const SVM_Value value);

/**
 * This function extracts the code contained in a symbol value.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] symbol The symbol value to extract from.
 *
 * \returns The SVM_Code contained in the value.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the symbol parameter does not contain an symbol.
 *
 * \see svm_value_type_is_symbol
 * \see svm_value_type_get
 */
SVM_FUNCTION SVM_Code svm_value_symbol_get_code(const void *svm, const SVM_Value_Symbol symbol);

/**
 * This function extracts the address contained in a symbol value.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] symbol The symbol value to extract from.
 *
 * \returns The address contained in the value.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the symbol parameter does not contain an symbol.
 *
 * \see svm_value_type_is_symbol
 * \see svm_value_type_get
 */
SVM_FUNCTION SVM_Address svm_value_symbol_get_address(const void *svm, const SVM_Value_Symbol symbol);

/**
 * This function returns the location string of the instruction referenced by a symbol.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] symbol The symbol referencing the instruction.
 *
 * \returns The string representation of the location of the instruction.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the symbol is not linked to an instruction.
 *
 * \see svm_processor_get_currentinstruction
 */
SVM_FUNCTION SVM_String svm_value_symbol_get_location(const void *svm, const SVM_Value_Symbol symbol);

/**
 * This function replaces the symbol value by another symbol value.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in,out] value The symbol value to update.
 * \param[in] symbol The symbol to use for update.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the value parameter does not contain an symbol.
 * \throws FAILURE interruption when the symbol parameter does not contain an symbol.
 * \throws FAILURE interruption when the value is a constant.
 *
 * \see svm_value_type_is_symbol
 * \see svm_value_type_get
 */
SVM_FUNCTION void svm_value_symbol_set(const void *svm, SVM_Value_Symbol value, const SVM_Value_Symbol symbol);

/**
 * This function replaces the code contained in a symbol value from a SVM_Code.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in,out] value The symbol value to update.
 * \param[in] code The new code value.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the value parameter does not contain an symbol.
 * \throws FAILURE interruption when the value is a constant.
 *
 * \see svm_value_type_is_symbol
 * \see svm_value_type_get
 */
SVM_FUNCTION void svm_value_symbol_set_code(const void *svm, SVM_Value_Symbol value, const SVM_Code code);

/**
 * This function replaces the address contained in a symbol value from a SVM_Address.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in,out] value The symbol value to update.
 * \param[in] address The new address value.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the value parameter does not contain an symbol.
 * \throws FAILURE interruption when the value is a constant.
 *
 * \see svm_value_type_is_symbol
 * \see svm_value_type_get
 */
SVM_FUNCTION void svm_value_symbol_set_address(const void *svm, SVM_Value_Symbol value, const SVM_Address address);

/**
 * This function replaces the code and the address contained in a symbol value from a SVM_Code and a SVM_Address.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in,out] value The symbol value to update.
 * \param[in] code The new code value.
 * \param[in] address The new address value.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the value parameter does not contain an symbol.
 * \throws FAILURE interruption when the value is a constant.
 *
 * \see svm_value_type_is_symbol
 * \see svm_value_type_get
 */
SVM_FUNCTION void svm_value_symbol_set_codeaddress(const void *svm, SVM_Value_Symbol value, const SVM_Code code, const SVM_Address address);

/** \} */

/** \defgroup pluginentrypointvalues SVM plugin entry point values
 * This module contains API functions to handle SVM plugin entry point values.
 * \{
 */

/**
 * This function creates a plugin entry point value the SVM can store in its memories from two string values.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] plugin_name The name of the plugin containing the plugin object.
 * \param[in] entry_name The name of the plugin object within the plugin.
 *
 * \returns The plugin entry point value.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Value_PluginEntryPoint svm_value_pluginentrypoint_new(const void *svm, const SVM_Value_String plugin_name, const SVM_Value_String entry_name);

/**
 * This function creates a plugin entry point value the SVM can store in its memories from two SVM_String.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] plugin_name The name of the plugin containing the plugin object.
 * \param[in] entry_name The name of the plugin object within the plugin.
 *
 * \returns The plugin entry point value.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Value_PluginEntryPoint svm_value_pluginentrypoint_new__string(const void *svm, const SVM_String plugin_name, const SVM_String entry_name);

/**
 * This function creates a plugin entry point value the SVM can store in its memories from two C nul-terminated strings.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] plugin_name The name of the plugin containing the plugin object.
 * \param[in] entry_name The name of the plugin object within the plugin.
 *
 * \returns The plugin entry point value.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Value_PluginEntryPoint svm_value_pluginentrypoint_new__raw(const void *svm, const char *plugin_name, const char *entry_name);

/**
 * This function creates a plugin entry point value the SVM can store in its memories from a plugin entry point and a C nul-terminated string.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] pep The plugin entry point to prefix.
 * \param[in] prefix The prefix to add to the entry name.
 *
 * The plugin entry point "<plugin name>.<entry name>" is transformed into "<plugin name>.<prefix>_<entry name>".
 *
 * \returns The plugin entry point value having a prefix.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Value_PluginEntryPoint svm_value_pluginentrypoint_new_prefix(const void *svm, const SVM_Value_PluginEntryPoint pep, const char *prefix);

/**
 * This function creates a plugin entry point value the SVM can store in its memories without a value.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 *
 * \returns A non initialised plugin entry point value.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Value_PluginEntryPoint svm_value_pluginentrypoint_new_null(const void *svm);

/**
 * This function checks whether a value contains a plugin entry point.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] value The value to be diagnosed.
 *
 * \returns TRUE if the value contains a plugin entry point, FALSE otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Boolean svm_value_type_is_pluginentrypoint(const void *svm, const SVM_Value value);

/**
 * This function extracts the plugin name contained in a plugin entry point value.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] pep The plugin entry point value to extract from.
 * 
 * \returns The plugin name contained in the value.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the pep parameter does not contain a plugin entry point.
 *
 * \see svm_value_type_is_pluginentrypoint
 * \see svm_value_type_get
 */
SVM_FUNCTION SVM_String svm_value_pluginentrypoint_get_plugin(const void *svm, const SVM_Value_PluginEntryPoint pep);

/**
 * This function extracts the entry name contained in a plugin entry point value.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] pep The plugin entry point value to extract from.
 * 
 * \returns The entry name contained in the value.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the pep parameter does not contain a plugin entry point.
 *
 * \see svm_value_type_is_pluginentrypoint
 * \see svm_value_type_get
 */
SVM_FUNCTION SVM_String svm_value_pluginentrypoint_get_entry(const void *svm, const SVM_Value_PluginEntryPoint pep);

/**
 * This function replaces the pluginentrypoint value by another pluginentrypoint value.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in,out] value The plugin entry point value to update.
 * \param[in] pluginentrypoint The plugin entry point to use for update.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the value parameter does not contain an plugin entry point.
 * \throws FAILURE interruption when the pluginentrypoint parameter does not contain an plugin entry point.
 * \throws FAILURE interruption when the value is a constant.
 *
 * \see svm_value_type_is_pluginentrypoint
 * \see svm_value_type_get
 */
SVM_FUNCTION void svm_value_pluginentrypoint_set(const void *svm, SVM_Value_PluginEntryPoint value, const SVM_Value_PluginEntryPoint pluginentrypoint);

/**
 * This function replaces the plugin name contained in a plugin entry point value from a string value.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in,out] value The plugin entry point value to update.
 * \param[in] plugin_name The new plugin name.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the value parameter does not contain a plugin entry point.
 * \throws FAILURE interruption when the value is a constant.
 *
 * \see svm_value_type_is_pluginentrypoint
 * \see svm_value_type_get
 */
SVM_FUNCTION void svm_value_pluginentrypoint_set_plugin(const void *svm, SVM_Value_PluginEntryPoint value, const SVM_Value_String plugin_name);

/**
 * This function replaces the plugin name contained in a plugin entry point value from a SVM_String.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in,out] value The plugin entry point value to update.
 * \param[in] plugin_name The new plugin name.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the value parameter does not contain a plugin entry point.
 * \throws FAILURE interruption when the value is a constant.
 *
 * \see svm_value_type_is_pluginentrypoint
 * \see svm_value_type_get
 */
SVM_FUNCTION void svm_value_pluginentrypoint_set_plugin__string(const void *svm, SVM_Value_PluginEntryPoint value, const SVM_String plugin_name);

/**
 * This function replaces the plugin name contained in a plugin entry point value from a C nul-terminated string.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in,out] value The plugin entry point value to update.
 * \param[in] plugin_name The new plugin name.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the value parameter does not contain a plugin entry point.
 * \throws FAILURE interruption when the value is a constant.
 *
 * \see svm_value_type_is_pluginentrypoint
 * \see svm_value_type_get
 */
SVM_FUNCTION void svm_value_pluginentrypoint_set_plugin__raw(const void *svm, SVM_Value_PluginEntryPoint value, const char *plugin_name);

/**
 * This function replaces the entry name contained in a plugin entry point value from a string value and an optional C nul-terminated string prefix.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in,out] value The plugin entry point value to update.
 * \param[in] entry_name The new entry name.
 * \param[in] prefix An optional prefix. The prefix is ignored when it is a null pointer.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the value parameter does not contain a plugin entry point.
 * \throws FAILURE interruption when the value is a constant.
 *
 * \see svm_value_type_is_pluginentrypoint
 * \see svm_value_type_get
 */
SVM_FUNCTION void svm_value_pluginentrypoint_set_entry(const void *svm, SVM_Value_PluginEntryPoint value, const SVM_Value_String entry_name, const char *prefix);

/**
 * This function replaces the entry name contained in a plugin entry point value from a SVM_String and an optional C nul-terminated string prefix.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in,out] value The plugin entry point value to update.
 * \param[in] entry_name The new entry name.
 * \param[in] prefix An optional prefix. The prefix is ignored when it is a null pointer.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the value parameter does not contain a plugin entry point.
 * \throws FAILURE interruption when the value is a constant.
 *
 * \see svm_value_type_is_pluginentrypoint
 * \see svm_value_type_get
 */
SVM_FUNCTION void svm_value_pluginentrypoint_set_entry__string(const void *svm, SVM_Value_PluginEntryPoint value, const SVM_String entry_name, const char *prefix);

/**
 * This function replaces the entry name contained in a plugin entry point value from two C nul-terminated strings.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in,out] value The plugin entry point value to update.
 * \param[in] entry_name The new entry name.
 * \param[in] prefix An optional prefix. The prefix is ignored when it is a null pointer.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the value parameter does not contain a plugin entry point.
 * \throws FAILURE interruption when the value is a constant.
 *
 * \see svm_value_type_is_pluginentrypoint
 * \see svm_value_type_get
 */
SVM_FUNCTION void svm_value_pluginentrypoint_set_entry__raw(const void *svm, SVM_Value_PluginEntryPoint value, const char *entry_name, const char *prefix);

/**
 * This function replaces the plugin name and the entry name contained in a plugin entry point value from two string values and an optional C nul-terminated string prefix.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in,out] value The plugin entry point value to update.
 * \param[in] plugin_name The new plugin name.
 * \param[in] entry_name The new entry name.
 * \param[in] prefix An optional prefix. The prefix is ignored when it is a null pointer.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the value parameter does not contain a plugin entry point.
 * \throws FAILURE interruption when the value is a constant.
 *
 * \see svm_value_type_is_pluginentrypoint
 * \see svm_value_type_get
 */
SVM_FUNCTION void svm_value_pluginentrypoint_set_pluginentry(const void *svm, SVM_Value_PluginEntryPoint value, const SVM_Value_String plugin_name, const SVM_Value_String entry_name, const char *prefix);

/**
 * This function replaces the plugin name and the entry name contained in a plugin entry point value from two SVM_String and an optional C nul-terminated string prefix.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in,out] value The plugin entry point value to update.
 * \param[in] plugin_name The new plugin name.
 * \param[in] entry_name The new entry name.
 * \param[in] prefix An optional prefix. The prefix is ignored when it is a null pointer.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the value parameter does not contain a plugin entry point.
 * \throws FAILURE interruption when the value is a constant.
 *
 * \see svm_value_type_is_pluginentrypoint
 * \see svm_value_type_get
 */
SVM_FUNCTION void svm_value_pluginentrypoint_set_pluginentry__string(const void *svm, SVM_Value_PluginEntryPoint value, const SVM_String plugin_name, const SVM_String entry_name, const char *prefix);

/**
 * This function replaces the plugin name and the entry name contained in a plugin entry point value from three C nul-terminated strings.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in,out] value The plugin entry point value to update.
 * \param[in] plugin_name The new plugin name.
 * \param[in] entry_name The new entry name.
 * \param[in] prefix An optional prefix. The prefix is ignored when it is a null pointer.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the value parameter does not contain a plugin entry point.
 * \throws FAILURE interruption when the value is a constant.
 *
 * \see svm_value_type_is_pluginentrypoint
 * \see svm_value_type_get
 */
SVM_FUNCTION void svm_value_pluginentrypoint_set_pluginentry__raw(const void *svm, SVM_Value_PluginEntryPoint value, const char *plugin_name, const char *entry_name, const char *prefix);

/** \} */

/** \defgroup interruptionvalues SVM interruption values
 * This module contains API functions to handle SVM interruption values.
 * \{
 */

/**
 * This function creates an interruption the SVM can store in its memories from an interruption type and a string value.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] interruption The interruption code.
 * \param[in] message A string for the SVM user.
 * \param[in] kind The interruption type.
 *
 * The created interruption has an internal type and can be set as an hardware interruption.
 *
 * \returns The interruption value.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the interruption is set to PLUGIN_INTERRUPTION.
 */
SVM_FUNCTION SVM_Value_Interruption svm_value_interruption_new_internal(const void *svm, const SVM_Interruption_Internal interruption, const SVM_Value_String message, const SVM_Interruption_Kind kind);

/**
 * This function creates an interruption the SVM can store in its memories from an interruption type and a SVM_String.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] interruption The interruption code.
 * \param[in] message A string for the SVM user.
 * \param[in] kind The interruption type.
 *
 * The created interruption has an internal type and can be set as an hardware interruption.
 *
 * \returns The interruption value.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the interruption is set to PLUGIN_INTERRUPTION.
 */
SVM_FUNCTION SVM_Value_Interruption svm_value_interruption_new_internal__string(const void *svm, const SVM_Interruption_Internal interruption, const SVM_String message, const SVM_Interruption_Kind kind);

/**
 * This function creates an interruption the SVM can store in its memories from an interruption type and a C nul-terminated string.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] interruption The interruption code.
 * \param[in] message A string for the SVM user.
 * \param[in] kind The interruption type.
 *
 * The created interruption has an internal type and can be set as an hardware interruption.
 *
 * \returns The interruption value.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the interruption is set to PLUGIN_INTERRUPTION.
 */
SVM_FUNCTION SVM_Value_Interruption svm_value_interruption_new_internal__raw(const void *svm, const SVM_Interruption_Internal interruption, const char *message, const SVM_Interruption_Kind kind);

/**
 * This function creates an interruption the SVM can store in its memories from a plugin entry point and a string value.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] name The interruption extended type.
 * \param[in] message A string for the SVM user.
 *
 * The created interruption has a type defined in a plugin.
 * The interruption type is automatically set to PLUGIN_INTERRUPTION.
 *
 * \returns The interruption value.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the name parameter does not identify a plugin interruption.
 */
SVM_FUNCTION SVM_Value_Interruption svm_value_interruption_new_external(const void *svm, const SVM_Value_PluginEntryPoint name, const SVM_Value_String message);

/**
 * This function creates an interruption the SVM can store in its memories from a plugin entry point and a SVM_String.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] name The interruption extended type.
 * \param[in] message A string for the SVM user.
 *
 * The created interruption has a type defined in a plugin.
 * The interruption type is automatically set to PLUGIN_INTERRUPTION.
 *
 * \returns The interruption value.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the name parameter does not identify a plugin interruption.
 */
SVM_FUNCTION SVM_Value_Interruption svm_value_interruption_new_external__string(const void *svm, const SVM_Value_PluginEntryPoint name, const SVM_String message);

/**
 * This function creates an interruption the SVM can store in its memories from a plugin entry point and a C nul-terminated string.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] name The interruption extended type.
 * \param[in] message A string for the SVM user.
 *
 * The created interruption has a type defined in a plugin.
 * The interruption type is automatically set to PLUGIN_INTERRUPTION.
 *
 * \returns The interruption value.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the name parameter does not identify a plugin interruption.
 */
SVM_FUNCTION SVM_Value_Interruption svm_value_interruption_new_external__raw(const void *svm, const SVM_Value_PluginEntryPoint name, const char *message);

/**
 * This function creates an interruption value the SVM can store in its memories without a value.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 *
 * \returns A non initialised interruption value.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Value_Interruption svm_value_interruption_new_null(const void *svm);

/**
 * This function checks whether a value contains an interruption.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] value The value to be diagnosed.
 *
 * \returns TRUE if the value contains an interruption, FALSE otherwise.
 * *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Boolean svm_value_type_is_interruption(const void *svm, const SVM_Value value);

/**
 * This function extracts the interruption type contained in an interruption value.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] interruption The interruption value to extract from.
 *
 * \returns TRUE when the interruption is internal, FALSE otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the interruption parameter does not contain an interruption.
 * \throws FAILURE interruption when the interruption is external.
 *
 * \see svm_value_type_is_interruption
 * \see svm_value_type_get
 */
SVM_FUNCTION SVM_Boolean svm_value_interruption_is_internal(const void *svm, const SVM_Value_Interruption interruption);

/**
 * This function extracts the interruption extension name contained in an interruption value.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] interruption The interruption value to extract from.
 *
 * \returns TRUE when the interruption is external, FALSE otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the interruption parameter does not contain an interruption.
 * \throws FAILURE interruption when the interruption is internal.
 *
 * \see svm_value_type_is_interruption
 * \see svm_value_type_get
 */
SVM_FUNCTION SVM_Boolean svm_value_interruption_is_external(const void *svm, const SVM_Value_Interruption interruption);

/**
 * This function extracts the interruption type contained in an interruption value.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] interruption The interruption value to extract from.
 *
 * \returns The interruption type contained in the value.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the interruption parameter does not contain an interruption.
 * \throws FAILURE interruption when the interruption is external.
 *
 * \see svm_value_type_is_interruption
 * \see svm_value_type_get
 */
SVM_FUNCTION SVM_Interruption_Internal svm_value_interruption_get_internal(const void *svm, const SVM_Value_Interruption interruption);

/**
 * This function extracts the interruption extension name contained in an interruption value.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] interruption The interruption value to extract from.
 *
 * \returns The interruption extension name contained in the value.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the interruption parameter does not contain an interruption.
 * \throws FAILURE interruption when the interruption is internal.
 *
 * \see svm_value_type_is_interruption
 * \see svm_value_type_get
 */
SVM_FUNCTION SVM_Value_PluginEntryPoint svm_value_interruption_get_external(const void *svm, const SVM_Value_Interruption interruption);

/**
 * This function extracts the interruption message contained in an interruption value.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] interruption The interruption value to extract from.
 *
 * \returns The interruption message contained in the value.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the interruption parameter does not contain an interruption.
 *
 * \see svm_value_type_is_interruption
 * \see svm_value_type_get
 */
SVM_FUNCTION SVM_String svm_value_interruption_get_message(const void *svm, const SVM_Value_Interruption interruption);

/**
 * This function extracts the interruption kind contained in an interruption value.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] interruption The interruption value to extract from.
 *
 * \returns The interruption kind contained in the value.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the interruption parameter does not contain an interruption.
 *
 * \see svm_value_type_is_interruption
 * \see svm_value_type_get
 */
SVM_FUNCTION SVM_Interruption_Kind svm_value_interruption_get_kind(const void *svm, const SVM_Value_Interruption interruption);

/**
 * This function replaces the interruption value by another interruption value.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in,out] value The interruption value to update.
 * \param[in] interruption The interruption to use for update.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the value parameter does not contain an interruption.
 * \throws FAILURE interruption when the interruption parameter does not contain an interruption.
 * \throws FAILURE interruption when the value is a constant.
 *
 * \see svm_value_type_is_interruption
 * \see svm_value_type_get
 */
SVM_FUNCTION void svm_value_interruption_set(const void *svm, SVM_Value_Interruption value, const SVM_Value_Interruption interruption);

/**
 * This function replaces the interruption type, the message and the kind of interruption contained in an interruption value from specific values and a string value message.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in,out] value The interruption value to update.
 * \param[in] type The new interruption type.
 * \param[in] message The new interruption message.
 * \param[in] kind The new interruption kind.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the value parameter does not contain an interruption.
 * \throws FAILURE interruption when the value is a constant.
 * \throws FAILURE interruption when the interruption type is PLUGIN_INTERRUPTION.
 *
 * \see svm_value_type_is_interruption
 * \see svm_value_type_get
 */
SVM_FUNCTION void svm_value_interruption_set_internal(const void *svm, SVM_Value_Interruption value, const SVM_Interruption_Internal type, const SVM_Value_String message, const SVM_Interruption_Kind kind);

/**
 * This function replaces the interruption type, the message and the kind of interruption contained in an interruption value from specific values and a SVM_String message.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in,out] value The interruption value to update.
 * \param[in] type The new interruption type.
 * \param[in] message The new interruption message.
 * \param[in] kind The new interruption kind.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the value parameter does not contain an interruption.
 * \throws FAILURE interruption when the value is a constant.
 * \throws FAILURE interruption when the interruption type is PLUGIN_INTERRUPTION.
 *
 * \see svm_value_type_is_interruption
 * \see svm_value_type_get
 */
SVM_FUNCTION void svm_value_interruption_set_internal__string(const void *svm, SVM_Value_Interruption value, const SVM_Interruption_Internal type, const SVM_String message, const SVM_Interruption_Kind kind);

/**
 * This function replaces the interruption type, the message and the kind of interruption contained in an interruption value from specific values and a C nul-terminated string message.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in,out] value The interruption value to update.
 * \param[in] type The new interruption type.
 * \param[in] message The new interruption message.
 * \param[in] kind The new interruption kind.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the value parameter does not contain an interruption.
 * \throws FAILURE interruption when the value is a constant.
 * \throws FAILURE interruption when the interruption type is PLUGIN_INTERRUPTION.
 *
 * \see svm_value_type_is_interruption
 * \see svm_value_type_get
 */
SVM_FUNCTION void svm_value_interruption_set_internal__raw(const void *svm, SVM_Value_Interruption value, const SVM_Interruption_Internal type, const char *message, const SVM_Interruption_Kind kind);

/**
 * This function replaces the interruption name and the message contained in an interruption value from specific values and a string value message.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in,out] value The interruption value to update.
 * \param[in] name The new interruption extension name.
 * \param[in] message The new interruption message.
 *
 * The interruption type is automatically set to PLUGIN_INTERRUPTION.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the value parameter does not contain an interruption.
 * \throws FAILURE interruption when the name parameter does not identify a plugin interruption.
 * \throws FAILURE interruption when the value is a constant.
 *
 * \see svm_value_type_is_interruption
 * \see svm_value_type_get
 */
SVM_FUNCTION void svm_value_interruption_set_external(const void *svm, SVM_Value_Interruption value, const SVM_Value_PluginEntryPoint name, const SVM_Value_String message);

/**
 * This function replaces the interruption name and the message contained in an interruption value from specific values and a SVM_String message.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in,out] value The interruption value to update.
 * \param[in] name The new interruption extension name.
 * \param[in] message The new interruption message.
 *
 * The interruption type is automatically set to PLUGIN_INTERRUPTION.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the value parameter does not contain an interruption.
 * \throws FAILURE interruption when the name parameter does not identify a plugin interruption.
 * \throws FAILURE interruption when the value is a constant.
 *
 * \see svm_value_type_is_interruption
 * \see svm_value_type_get
 */
SVM_FUNCTION void svm_value_interruption_set_external__string(const void *svm, SVM_Value_Interruption value, const SVM_Value_PluginEntryPoint name, const SVM_String message);

/**
 * This function replaces the interruption name and the message contained in an interruption value from specific values and a C nul-terminated string message.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in,out] value The interruption value to update.
 * \param[in] name The new interruption extension name.
 * \param[in] message The new interruption message.
 *
 * The interruption type is automatically set to PLUGIN_INTERRUPTION.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the value parameter does not contain an interruption.
 * \throws FAILURE interruption when the name parameter does not identify a plugin interruption.
 * \throws FAILURE interruption when the value is a constant.
 *
 * \see svm_value_type_is_interruption
 * \see svm_value_type_get
 */
SVM_FUNCTION void svm_value_interruption_set_external__raw(const void *svm, SVM_Value_Interruption value, const SVM_Value_PluginEntryPoint name, const char *message);

/** \} */

/** \defgroup pluginvalues SVM plugin values
 * This module contains API functions to handle SVM plugin values.
 * \{
 */

/**
 * This function creates a plugin defined value the SVM can store in its memories from a plugin entry point and generic C pointer.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] type The extended type name.
 * \param[in] value The raw value pointer.
 *
 * \note If the type does not support the copy operation, any write into memory attempt will fail unless the value is flagged movable.
 *
 * \returns The plugin defined value.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws MEMORY interruption when the type does not exist.
 * \see svm_value_state_set_movable
 */
SVM_FUNCTION SVM_Value_Plugin svm_value_plugin_new(const void *svm, const SVM_Value_PluginEntryPoint type, void *value);

/**
 * This function creates a plugin value the SVM can store in its memories without a value.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] type The plugin type of the null value.
 *
 * \returns A non initialised plugin value of specified type.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws MEMORY interruption when the type does not exist.
 */
SVM_FUNCTION SVM_Value_Plugin svm_value_plugin_new_null(const void *svm, const SVM_Value_PluginEntryPoint type);

/**
 * This function creates a plugin defined value the SVM can store in its memories from a plugin entry point and a string value.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] type The extended type name.
 * \param[in] value The string representation of the value.
 *
 * \note If the type does not support the copy operation, any write into memory attempt will fail unless the value is flagged movable.
 *
 * \returns The plugin defined value.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws MEMORY interruption when the type does not exist or when not constant constructible.
 */
SVM_FUNCTION SVM_Value_Plugin svm_value_plugin_new_const(const void *svm, const SVM_Value_PluginEntryPoint type, const SVM_Value_String value);

/**
 * This function creates a plugin defined value the SVM can store in its memories from a plugin entry point and a string.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] type The extended type name.
 * \param[in] value The string representation of the value.
 *
 * \note If the type does not support the copy operation, any write into memory attempt will fail unless the value is flagged movable.
 *
 * \returns The plugin defined value.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws MEMORY interruption when the type does not exist or when not constant constructible.
 */
SVM_FUNCTION SVM_Value_Plugin svm_value_plugin_new_const__string(const void *svm, const SVM_Value_PluginEntryPoint type, const SVM_String value);

/**
 * This function creates a plugin defined value the SVM can store in its memories from a plugin entry point and a raw C string.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] type The extended type name.
 * \param[in] value The string representation of the value.
 *
 * \note If the type does not support the copy operation, any write into memory attempt will fail unless the value is flagged movable.
 *
 * \returns The plugin defined value.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws MEMORY interruption when the type does not exist or when not constant constructible.
 */
SVM_FUNCTION SVM_Value_Plugin svm_value_plugin_new_const__raw(const void *svm, const SVM_Value_PluginEntryPoint type, const char *value);

/**
 * This function checks whether a value contains a plugin value of a given type.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] value The value to be diagnosed.
 * \param[in] type The plugin type.
 *
 * \returns TRUE if the value contains a plugin value of the specified type, FALSE otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws MEMORY interruption when the type does not exist.
 */
SVM_FUNCTION SVM_Boolean svm_value_type_is_plugin(const void *svm, const SVM_Value value, const SVM_Value_PluginEntryPoint type);

/**
 * This function extracts the plugin value type contained in a plugin value.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] plugin The plugin value to extract from.
 *
 * \returns The plugin value type of the plugin value.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the value parameter does not contain a plugin value.
 *
 * \see svm_value_type_is_plugin
 * \see svm_value_type_get
 */
SVM_FUNCTION SVM_Value_PluginEntryPoint svm_value_plugin_get_type(const void *svm, const SVM_Value_Plugin plugin);

/**
 * This function extracts the plugin value contained in a plugin value.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] plugin The plugin value to extract from.
 *
 * \returns The plugin raw value of the plugin value.
 *
 * \note Any change done on the value returned by this function will not be synchronised with the debugger until the end of the callback. The API function svm_debug_synchronise allows explicit synchronisation.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the value parameter does not contain a plugin value.
 *
 * \see svm_value_type_is_plugin
 * \see svm_value_type_get
 * \see svm_debug_synchronise
 */
SVM_FUNCTION void* svm_value_plugin_get_internal(const void *svm, const SVM_Value_Plugin plugin);

/**
 * This function replaces the plugin value by another plugin value.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in,out] value The plugin value to update.
 * \param[in] plugin The plugin to use for update.
 *
 * \warning The previous value of the updated plugin value is deleted. The new value is copied into the value.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the value parameter does not contain a plugin value.
 * \throws FAILURE interruption when the value parameter can not be deleted.
 * \throws FAILURE interruption when the plugin parameter does not contain a plugin value.
 * \throws FAILURE interruption when the plugin parameter internal value can not be copied.
 * \throws FAILURE interruption when the value and plugin parameters have different types.
 * \throws FAILURE interruption when the value is a constant.
 *
 * \see svm_value_type_is_interruption
 * \see svm_value_type_get
 */
SVM_FUNCTION void svm_value_plugin_set(const void *svm, SVM_Value_Plugin value, const SVM_Value_Plugin plugin);

/**
 * This function replaces the type and the value contained in a plugin value from specific values.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in,out] value The interruption value to update.
 * \param[in] type The plugin value type.
 * \param[in] internal_value The plugin raw value.
 *
 * \warning The previous value of the updated plugin value is deleted.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the value parameter does not contain a plugin value.
 * \throws FAILURE interruption when the value parameter can not be deleted.
 * \throws FAILURE interruption when the type is different from the value one.
 * \throws FAILURE interruption when the value is a constant.
 * \throws MEMORY interruption when the type does not identify a plugin type.
 *
 * \see svm_value_type_is_plugin
 * \see svm_value_type_get
 * \see svm_plugin_has_type
 */
SVM_FUNCTION void svm_value_plugin_set_internal(const void *svm, SVM_Value_Plugin value, const SVM_Value_PluginEntryPoint type, void *internal_value);

/**
 * This function transforms a SVM_Comparison_Result into a SVM_Value_Plugin_Comparison.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] compare The value to be transformed.
 *
 * \returns The plugin comparison from the comparison value.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 *
 * \see SVM_Comparison_Result
 * \see SVM_Value_Plugin_Comparison
 */
SVM_FUNCTION SVM_Value_Plugin_Comparison svm_value_plugin_compare_convert(const void *svm, const SVM_Comparison_Result compare);

/** \} */

/** \defgroup automaticvalues SVM automatic values
 * This module contains API functions to handle SVM automatic values.
 * \{
 */

/**
 * This function creates an automatic value the SVM can store in its memories without a value.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 *
 * \returns A non initialised automatic value.
 *
 * \note Writing into a memory address a null automatic value will clear the value without changing its type, and will work with all types.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Value_Automatic svm_value_automatic_new_null(const void *svm);

/** \} */

/** \defgroup valuestypes SVM values types
 * This module contains API functions to handle SVM values types.
 * \{
 */

/**
 * This function extracts the type from a value.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] value The value from which the type is extracted.
 *
 * \returns The type of the value.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Type svm_value_type_get(const void *svm, const SVM_Value value);

/**
 * This function prints the type from a value.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] value The value from which the type is extracted.
 *
 * \returns The string representation of the type of the value.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_String svm_value_type_print(const void *svm, const SVM_Value value);

/** \} */

/**
 * This function compares two values.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] left The left operand of the equality.
 * \param[in] right The right operand of the equality.
 *
 * \returns The comparison status of the two values.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 *
 * \see SVM_Comparison_Result
 */
SVM_FUNCTION SVM_Comparison_Result svm_value_compare(const void *svm, const SVM_Value left, const SVM_Value right);

/**
 * This function checks whether two values are equal.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] left The left operand of the equality.
 * \param[in] right The right operand of the equality.
 *
 * \returns TRUE if the two values have the same type and the same value, FALSE otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 *
 * \see svm_value_compare
 */
SVM_FUNCTION SVM_Boolean svm_value_is_equal(const void *svm, const SVM_Value left, const SVM_Value right);

/** \defgroup valuesstate SVM values state
 * This module contains API functions to handle SVM values state.
 * \{
 */

/**
 * This function checks whether a value is a null value.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] value The value to be diagnosed.
 *
 * \returns TRUE if the value is a null value, FALSE otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Boolean svm_value_state_is_null(const void *svm, const SVM_Value value);

/**
 * This function checks whether a value is a constant.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] value The value to be diagnosed.
 *
 * \returns TRUE if the value is a constant, FALSE otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Boolean svm_value_state_is_constant(const void *svm, const SVM_Value value);

/**
 * This function checks whether a value is movable.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] value The value to be diagnosed.
 *
 * \returns TRUE if the value will be moved at its next write into memory, FALSE otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Boolean svm_value_state_is_movable(const void *svm, const SVM_Value value);

/**
 * This function flags a value as movable.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] value The value to be flagged.
 *
 * The value is flagged movable only when the value is referenced once.
 * When a value is flagged as movable, the next write into the memory is done without copy.
 *
 * \returns TRUE when the value has been flagged, FALSE otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Boolean svm_value_state_set_movable(const void *svm, const SVM_Value value);

/** \} */

/**
 * This function creates a copy from a value.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] value The value to be copied.
 *
 * \note If a value can not be copied, a MEMORY interruption is raised.
 *
 * \returns The non-constant copy of the value.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws MEMORY interruption when the value can not be copied.
 */
SVM_FUNCTION SVM_Value svm_value_copy(const void *svm, const SVM_Value value);

/**
 * This function extracts a string representation of a value.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] value The value to be rendered as string.
 *
 * \note If a value can not correctly rendered, an ugly default rendering is returned instead.
 *
 * \returns The string representation of the value.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_String svm_value_print(const void *svm, const SVM_Value value);

/**
 * This function creates an array of values.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] size The size of the array.
 *
 * \returns A pointer to a C array of SVM_Value. Every cell of the array has to be initialised by a value before being passed to an API function.
 *
 * \note The returned array is a valid SVM variable: its scope can be changed if needed, and the pointer shall not be freed explicitely.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Value* svm_value_array_new(const void *svm, SVM_Size size);

/** \} */

/** \defgroup codes SVM codes
 * This module contains API functions to handle SVM codes.
 *
 * SVM code are objects containing execution directives for processors, written by the virtual machine users.
 *
 * This concept allows usage of different consistent modules having a dedicated scope to:
 * - Create fully independant libraries,
 * - Create local code contextes to split application in components with identified entry points,
 * - Introduce genericity in code execution by importing code as text,
 * - Use of code generators.
 *
 * \{
 */

/** \defgroup codeget SVM codes getters
 * This module contains API functions to create SVM codes.
 * \{
 */

/**
 * This function creates a SVM code from two string values.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] name The code name.
 * \param[in] source The code source.
 *
 * The source code is compiled and linked by the SVM code parser.
 *
 * \returns The executable SVM code.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when a parse error is detected.
 * \throws FAILURE interruption when a label or a symbol is defined twice or not found.
 * \throws MEMORY interruption when a plugin type does not exists or has no constant callback defined.
 * \throws PROCESSOR interruption when a plugin instruction or interruption does not exists.
 */
SVM_FUNCTION SVM_Code svm_code_new(const void *svm, const SVM_Value_String name, const SVM_Value_String source);

/**
 * This function creates a SVM code from two SVM_String.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] name The code name.
 * \param[in] source The code source.
 *
 * The source code is compiled and linked by the SVM code parser.
 *
 * \returns The executable SVM code.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when a parse error is detected.
 * \throws FAILURE interruption when a label or a symbol is defined twice or not found.
 * \throws MEMORY interruption when a plugin type does not exists or has no constant callback defined.
 * \throws PROCESSOR interruption when a plugin instruction or interruption does not exists.
 */
SVM_FUNCTION SVM_Code svm_code_new__string(const void *svm, const SVM_String name, const SVM_String source);

/**
 * This function creates a SVM code from two C nul-terminated strings.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] name The code name.
 * \param[in] source The code source.
 *
 * The source code is compiled and linked by the SVM code parser.
 *
 * \returns The executable SVM code.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when a parse error is detected.
 * \throws FAILURE interruption when a label or a symbol is defined twice or not found.
 * \throws MEMORY interruption when a plugin type does not exists or has no constant callback defined.
 * \throws PROCESSOR interruption when a plugin instruction or interruption does not exists.
 */
SVM_FUNCTION SVM_Code svm_code_new__raw(const void *svm, const char *name, const char *source);

/**
 * This function tries to create a SVM code from two string values.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] name The code name.
 * \param[in] source The code source.
 *
 * The source code is compiled and linked by the SVM code parser.
 *
 * \returns The executable SVM code (a SVM_Code) when the compilation succeed, or a string value (a SVM_Value_String) containing an error when it failed. 
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Variable svm_code_compile(const void *svm, const SVM_Value_String name, const SVM_Value_String source);

/**
 * This function creates a SVM code from two SVM_String.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] name The code name.
 * \param[in] source The code source.
 *
 * The source code is compiled and linked by the SVM code parser.
 *
 * \returns The executable SVM code (a SVM_Code) when the compilation succeed, or a string value (a SVM_Value_String) containing an error when it failed. 
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Variable svm_code_compile__string(const void *svm, const SVM_String name, const SVM_String source);

/**
 * This function creates a SVM code from two C nul-terminated strings.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] name The code name.
 * \param[in] source The code source.
 *
 * The source code is compiled and linked by the SVM code parser.
 *
 * \returns The executable SVM code (a SVM_Code) when the compilation succeed, or a string value (a SVM_Value_String) containing an error when it failed. 
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Variable svm_code_compile__raw(const void *svm, const char *name, const char *source);

/** \} */

/**
 * This function extracts the code name from the SVM code.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] code The code from wich the name has to be extracted.
 *
 * \returns The code name.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_String svm_code_get_name(const void *svm, const SVM_Code code);

/**
 * This function extracts the code source from the SVM code.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] code The code from wich the name has to be extracted.
 *
 * \returns The code source.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_String svm_code_get_source(const void *svm, const SVM_Code code);

/**
 * This function renders a textual representation of a SVM code.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] code The code to render as string.
 *
 * \note The resulting string is not guaranteed to be compilable again.
 * In particular, it contains a one line header that should be removed prior to any compilation attempt.
 *
 * \returns The string representation of a SVM code.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_String svm_code_print(const void *svm, const SVM_Code code);

/** \defgroup labelcodes SVM labels in code
 * This module contains API functions to handle labels in SVM code.
 * \{
 */

/**
 * This function checks whether a SVM code defines a label given as a string value.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] code The code to be diagnosed.
 * \param[in] label The label to check.
 *
 * \returns TRUE if the label is eligible to a local call in the SVM code, FALSE otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Boolean svm_code_label_has_address(const void *svm, const SVM_Code code, const SVM_Value_String label);

/**
 * This function checks whether a SVM code defines a label given as a SVM_String.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] code The code to be diagnosed.
 * \param[in] label The label to check.
 *
 * \returns TRUE if the label is eligible to a local call in the SVM code, FALSE otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Boolean svm_code_label_has_address__string(const void *svm, const SVM_Code code, const SVM_String label);

/**
 * This function checks whether a SVM code defines a label given as a C nul-terminated string.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] code The code to be diagnosed.
 * \param[in] label The label to check.
 *
 * \returns TRUE if the label is eligible to a local call in the SVM code, FALSE otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Boolean svm_code_label_has_address__raw(const void *svm, const SVM_Code code, const char *label);

/**
 * This function returns all labels defined in a SVM code.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] code The code to be introspected.
 *
 * This function returns all labels of a SVM code. Symbols are not returned by this function.
 *
 * \note The returned array is managed by the SVM and does not need to be freed/deleted.
 *
 * \returns A NULL terminated array of string values containing the label names.
 *
 * \note The returned array is a valid SVM variable: its scope can be changed if needed, and the pointer shall not be freed explicitely.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 *
 * \see svm_code_symbol_list
 */
SVM_FUNCTION SVM_Value_String* svm_code_label_list(const void *svm, const SVM_Code code);

/**
 * This function solves a label in a SVM code given as a string value.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] code The code to be diagnosed.
 * \param[in] label The label to solve.
 *
 * \note Symbols can also be targets of local calls.
 *
 * \returns The local address corresponding to the label.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the label does not allow a local jump in the code.
 */
SVM_FUNCTION SVM_Address svm_code_label_get_address(const void *svm, const SVM_Code code, const SVM_Value_String label);

/**
 * This function solves a label in a SVM code given as a SVM_String.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] code The code to be diagnosed.
 * \param[in] label The label to solve.
 *
 * \note Symbols can also be targets of local calls.
 *
 * \returns The local address corresponding to the label.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the label does not allow a local jump in the code.
 */
SVM_FUNCTION SVM_Address svm_code_label_get_address__string(const void *svm, const SVM_Code code, const SVM_String label);

/**
 * This function solves a label in a SVM code given as a C nul-terminated string.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] code The code to be diagnosed.
 * \param[in] label The label to solve.
 *
 * \note Symbols can also be targets of local calls.
 *
 * \returns The local address corresponding to the label.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the label does not allow a local jump in the code.
 */
SVM_FUNCTION SVM_Address svm_code_label_get_address__raw(const void *svm, const SVM_Code code, const char *label);

/** \} */

/** \defgroup symbolcodes SVM symbols in code
 * This module contains API functions to handle symbols in SVM code.
 * \{
 */

/**
 * This function checks whether a SVM code defines a symbol given as a string value.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] code The code to be diagnosed.
 * \param[in] label The label to solve.
 *
 * \returns TRUE if the symbol is eligible to a global call in the SVM code, FALSE otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Boolean svm_code_symbol_has_address(const void *svm, const SVM_Code code, const SVM_Value_String label);

/**
 * This function checks whether a SVM code defines a symbol given as a SVM_String.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] code The code to be diagnosed.
 * \param[in] label The label to solve.
 *
 * \returns TRUE if the symbol is eligible to a global call in the SVM code, FALSE otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Boolean svm_code_symbol_has_address__string(const void *svm, const SVM_Code code, const SVM_String label);

/**
 * This function checks whether a SVM code defines a symbol given as a C nul-terminated string.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] code The code to be diagnosed.
 * \param[in] label The label to solve.
 *
 * \returns TRUE if the symbol is eligible to a global call in the SVM code, FALSE otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Boolean svm_code_symbol_has_address__raw(const void *svm, const SVM_Code code, const char *label);

/**
 * This function returns all symbols defined in a SVM code.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] code The code to be introspected.
 *
 * This function returns all symbols of a SVM code. Labels are not returned by this function.
 *
 * \returns A NULL terminated array of string values containing the symbol names.
 *
 * \note The returned array is a valid SVM variable: its scope can be changed if needed, and the pointer shall not be freed explicitely.
 *
 * Usage example:
 * \code{.c}
 * SVM_Value_String *label = svm_code_symbol_list(svm,code);
 * for( ; !label ; ++label) // we don't need to keep label on the first label
 * {
 * 	SVM_Value_Symbol symbol = svm_code_symbol_get_address(svm,code,*label);
 *	// ...
 * }
 * \endcode
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 *
 * \see svm_code_label_list
 */
SVM_FUNCTION SVM_Value_String* svm_code_symbol_list(const void *svm, const SVM_Code code);

/**
 * This function solves a symbol in a SVM code given as a string value.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] code The code to be diagnosed.
 * \param[in] label The label to solve.
 *
 * \returns The symbol value containing the global address corresponding to the label.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the label does not allow a global jump in the code.
 */
SVM_FUNCTION SVM_Value_Symbol svm_code_symbol_get_address(const void *svm, const SVM_Code code, const SVM_Value_String label);

/**
 * This function solves a symbol in a SVM code given as a SVM_String.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] code The code to be diagnosed.
 * \param[in] label The label to solve.
 *
 * \returns The symbol value containing the global address corresponding to the label.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the label does not allow a global jump in the code.
 */
SVM_FUNCTION SVM_Value_Symbol svm_code_symbol_get_address__string(const void *svm, const SVM_Code code, const SVM_String label);

/**
 * This function solves a symbol in a SVM code given as a C nul-terminated string.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] code The code to be diagnosed.
 * \param[in] label The label to solve.
 *
 * \returns The symbol value containing the global address corresponding to the label.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the label does not allow a global jump in the code.
 */
SVM_FUNCTION SVM_Value_Symbol svm_code_symbol_get_address__raw(const void *svm, const SVM_Code code, const char *label);

/** \} */

/** \defgroup codeinstructions SVM code instructions
 * This module contains API functions to handle SVM code instructions.
 *
 * \{
 */

/**
 * This function returns the number of instructions of a given SVM code.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] code The code to be diagnosed.
 *
 * Valid addresses for this code are from 0 to the number returned by this function minus one.
 *
 * \returns The number of instructions of the SVM code.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Size svm_code_get_size(const void *svm, const SVM_Code code);

/**
 * This function returns the text of the instruction at a given address of a given SVM code.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] code The code to be diagnosed.
 * \param[in] address The address of the instruction.
 *
 * \returns A SVM_String with the text of the instruction.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the address is outside the address range of the SVM code.
 */
SVM_FUNCTION SVM_String svm_code_instruction_get_text(const void *svm, const SVM_Code code, const SVM_Address address);

/**
 * This function returns whether the instruction at a given address of a given SVM code has the system flag.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] code The code to be diagnosed.
 * \param[in] address The address of the instruction.
 *
 * \returns TRUE when the instruction is system, FALSE otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the address is outside the address range of the SVM code.
 */
SVM_FUNCTION SVM_Boolean svm_code_instruction_is_system(const void *svm, const SVM_Code code, const SVM_Address address);

/**
 * This function returns whether the instruction at a given address of a given SVM code has the waiting flag.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] code The code to be diagnosed.
 * \param[in] address The address of the instruction.
 *
 * \returns TRUE when the instruction is waiting, FALSE otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the address is outside the address range of the SVM code.
 */
SVM_FUNCTION SVM_Boolean svm_code_instruction_is_waiting(const void *svm, const SVM_Code code, const SVM_Address address);

/**
 * This function returns the text of the location of the instruction at a given address of a given SVM code.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] code The code to be diagnosed.
 * \param[in] address The address of the instruction.
 *
 * \returns A SVM_String with the text of the location in the code text of the instruction.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the address is outside the address range of the SVM code.
 */
SVM_FUNCTION SVM_String svm_code_instruction_get_location(const void *svm, const SVM_Code code, const SVM_Address address);

/** \} */

/** \} */

/** \defgroup accesscontrol SVM access control
 * This module contains API functions to handle SVM access control.
 *
 * Access control represents processor and memory quotas used to avoid spurious code execution.
 *
 * These quotas are defined at kernel level and are used only in protected mode.
 *
 * Once a quota is set, the processor and the memory usage is monitored and checked against the quota.
 * When a usage overflow attempt is detected, a SECURITY interruption is raised.
 *
 * \see kernels
 *
 * \{
 */

/**
 * This function creates an access control quota for a SVM code execution.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] max_instructions The maximum instructions that can be executed. A NULL pointer means no limit.
 * \param[in] max_memory The maximum memory that can be allocated. A NULL pointer means no limit.
 *
 * \returns An access control with some execution limits.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_AccessControl svm_accesscontrol_new(const void *svm, const unsigned long int *max_instructions, const unsigned long int *max_memory);

/** \defgroup accesscontrolinstructions SVM access control instructions
 * This module contains API functions to handle SVM access control instructions limit.
 *
 * \{
 */

/**
 * This function checks whether a quota for a SVM code execution contains a limit for instructions.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] access_control The access control to check.
 *
 * \returns TRUE when the access control has an instructions limit, FALSE otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Boolean svm_accesscontrol_has_instructions(const void *svm, const SVM_AccessControl access_control);

/**
 * This function retrieves the remaining instructions counter from a quota for a SVM code execution.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] access_control The access control to check.
 *
 * \returns The number of remaining instructions to execute.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the access control has no instruction limit.
 *
 * \see svm_accesscontrol_has_instructions
 */
SVM_FUNCTION unsigned long int svm_accesscontrol_get_instructions(const void *svm, const SVM_AccessControl access_control);

/**
 * This function changes the remaining instructions counter within a quota for a SVM code execution.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] access_control The access control to update.
 * \param[in] instructions The number of remaining instructions to run.
 *
 * This function changes the number of remaining instructions on the access control:
 * - if the access control had no limit, a limit is set to the number of remaining instructions,
 * - the number of remaining instructions can not be increased, and the function will have no effect.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 *
 */
SVM_FUNCTION void svm_accesscontrol_set_instructions(const void *svm, SVM_AccessControl access_control, const unsigned long int instructions);

/** \} */

/** \defgroup accesscontrolmemory SVM access control memory
 * This module contains API functions to handle SVM access control memory limit.
 *
 * \{
 */

/**
 * This function checks whether a quota for a SVM code execution contains a limit for memory.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] access_control The access control to check.
 *
 * \returns TRUE when the access control has a memory limit, FALSE otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Boolean svm_accesscontrol_has_memory(const void *svm, const SVM_AccessControl access_control);

/**
 * This function retrieves the remaining memory counter from a quota for a SVM code execution.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] access_control The access control to check.
 *
 * \returns The number of remaining memory to allocate.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the access control has no memory limit.
 *
 * \see svm_accesscontrol_has_memory
 */
SVM_FUNCTION unsigned long int svm_accesscontrol_get_memory(const void *svm, const SVM_AccessControl access_control);

/**
 * This function changes the remaining memory counter within a quota for a SVM code execution.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] access_control The access control to update.
 * \param[in] memory The remaining memory to allocate.
 *
 * This function changes the number of remaining memory on the access control:
 * - if the access control had no limit, a limit is set to the number of remaining memory,
 * - the number of remaining memory can not be increased, and the function will have no effect.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 *
 */
SVM_FUNCTION void svm_accesscontrol_set_memory(const void *svm, SVM_AccessControl access_control, const unsigned long int memory);

/** \} */

/** \} */

/** \defgroup kernels SVM kernels
 * This module contains API functions to handle SVM kernels.
 *
 * A kernel is an atomic execution context within the virtual machine composed mainly of a processor and a memory.
 *
 * \note A kernel is a common entry point in this API to get access to various elements linked to it.
 *
 * Kernels are included in processes to manage concurrent executions.
 * 
 * \see processes
 * \see processors
 * \see memories
 *
 * \{
 */

/** \defgroup getkernels SVM kernel getters
 * This module contains API functions to get SVM kernels.
 * \{
 */

/**
 * This function creates a new kernel.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] transmit_interruptions If the kernel is interrupted, this flag asks the kernel to forward the interruption to the next kernel.
 * \param[in] last_return_is_shutdown This flag indicates whether an extra return on an empty return stack can be considered as a shutdown.
 * \param[in] protected_mode This flag indicates that the code should be executed in protected mode.
 * \param[in] access_control The limitations to apply to the code execution. A NULL pointer means no specific restriction.
 * \param[in] code The code to execute within the kernel.
 *
 * \note When this API function is called within a kernel, the protected mode and the access control are always set as restrictions of the current limitations.
 * In particular, if the current kernel executes code in protected mode, the created kernel will automatically be in protected mode, no matter the protected_mode flag could be.
 *
 * \note Access control is ignored when the kernel is not created in protected mode.
 *
 * \warning When this API function is called outside a kernel, the protected mode and access control parameters are directly applied.
 * This can lead to a security issue unless either the plugin callback containing such call can only be triggered by a system instruction, or the code is coming only from safe sources.
 *
 * \returns The created kernel.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when this API function is invoked outside a kernel and outside a system callback.
 *
 * \see accesscontrol
 * \see svm_kernel_get_current
 */
SVM_FUNCTION SVM_Kernel svm_kernel_new_code(const void *svm, const SVM_Boolean transmit_interruptions, const SVM_Boolean last_return_is_shutdown, const SVM_Boolean protected_mode, const SVM_AccessControl access_control, const SVM_Code code);

/**
 * This function creates a new kernel.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] transmit_interruptions If the kernel is interrupted, this flag asks the kernel to forward the interruption to the next kernel.
 * \param[in] last_return_is_shutdown This flag indicates whether an extra return on an empty return stack can be considered as a shutdown.
 * \param[in] protected_mode This flag indicates that the code should be executed in protected mode.
 * \param[in] access_control The limitations to apply to the code execution. A NULL pointer means no specific restriction.
 * \param[in] symbol The symbol to jump to within the kernel.
 *
 * \note When this API function is called within a kernel, the protected mode and the access control are always set as restrictions of the current limitations.
 * In particular, if the current kernel executes code in protected mode, the created kernel will automatically be in protected mode, no matter the protected_mode flag could be.
 *
 * \note Access control is ignored when the kernel is not created in protected mode.
 *
 * \warning When this API function is called outside a kernel, the protected mode and access control parameters are directly applied.
 * This can lead to a security issue unless either the plugin callback containing such call can only be triggered by a system instruction, or the code is coming only from safe sources.
 *
 * \returns The created kernel.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when this API function is invoked outside a kernel and outside a system callback.
 *
 * \see accesscontrol
 * \see svm_kernel_get_current
 */
SVM_FUNCTION SVM_Kernel svm_kernel_new_symbol(const void *svm, const SVM_Boolean transmit_interruptions, const SVM_Boolean last_return_is_shutdown, const SVM_Boolean protected_mode, const SVM_AccessControl access_control, const SVM_Value_Symbol symbol);

/**
 * This function retrieves the current kernel.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 *
 * \warning The resulting variable can be set global and added to a C/C++ structure linked to a type which can be written into the memory of the current kernel. In such case, a circular loop is created, preventing the variable to be destroyed at the proper time unless the variable is set local outside the type destructor or the value is explicitely removed from the kernel memory.
 *
 * \returns The current kernel if the API function is invoked inside a kernel, a NULL pointer otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Kernel svm_kernel_get_current(const void *svm);

/**
 * This function returns the process associated to a kernel.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel to diagnose.
 *
 * \returns The process associated to the kernel if attached to a process, NULL otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 *
 * \see processes
 */
SVM_FUNCTION SVM_Process svm_kernel_get_process(const void *svm, const SVM_Kernel kernel);

/** \} */

/** \defgroup statuskernels SVM kernel status
 * This module contains API functions to handle SVM kernel status.
 * \{
 */

/**
 * This function checks whether a kernel can be executed.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel to be diagnosed.
 *
 * \returns TRUE if the kernel can be returned to the process by a sequencer, FALSE otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 *
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 */
SVM_FUNCTION SVM_Boolean svm_kernel_is_runnable(const void* svm, const SVM_Kernel kernel);

/**
 * This function checks whether a kernel has completed its execution.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel to be diagnosed.
 *
 * \returns TRUE if the kernel is in zombie, interrupted or error state, FALSE otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 *
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 */
SVM_FUNCTION SVM_Boolean svm_kernel_is_terminated(const void* svm, const SVM_Kernel kernel);

/**
 * This function returns the status of the kernel.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel to be diagnosed.
 *
 * \returns The status of the kernel.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 *
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 */
SVM_FUNCTION SVM_Kernel_State svm_kernel_get_state(const void* svm, const SVM_Kernel kernel);

/**
 * This function checks whether a kernel has the transmit_interruptions option.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel to be diagnosed.
 *
 * \returns TRUE if the kernel has the transmit_interruptions option activated, FALSE otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 *
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 */
SVM_FUNCTION SVM_Boolean svm_kernel_has_transmitinterruption(const void* svm, const SVM_Kernel kernel);

/**
 * This function checks whether a kernel has the last_return_is_shutdown option.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel to be diagnosed.
 *
 * \returns TRUE if the kernel has the last_return_is_shutdown option activated, FALSE otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 *
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 */
SVM_FUNCTION SVM_Boolean svm_kernel_has_lastreturnisshutdown(const void* svm, const SVM_Kernel kernel);

/**
 * This function checks whether a kernel has the protected_mode option.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel to be diagnosed.
 *
 * \returns TRUE if the kernel has the protected_mode option activated, FALSE otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 *
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 */
SVM_FUNCTION SVM_Boolean svm_kernel_has_protectedmode(const void* svm, const SVM_Kernel kernel);

/**
 * This function retrieves the access control of a protected kernel.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel from which the interruption will be extracted.
 *
 * \returns The access control of the kernel if the kernel has a defined quota, a NULL pointer otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 *
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 */
SVM_FUNCTION SVM_AccessControl svm_kernel_get_accesscontrol(const void* svm, const SVM_Kernel kernel);

/**
 * This function modifies the access control of a protected kernel.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel from which the interruption will be extracted.
 * \param[in] access_control The access control to set.
 *
 * \note The resulting access control on the kernel is made of the lowest limit between the current access control of the kernel and the new one.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 *
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 */
SVM_FUNCTION void svm_kernel_set_accesscontrol(const void* svm, const SVM_Kernel kernel, const SVM_AccessControl access_control);

/**
 * This function retrieves the interruption of an interrupted kernel.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel from which the interruption will be extracted.
 *
 * \returns If the kernel is interrupted, the interruption of the kernel, a NULL pointer otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 *
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 */
SVM_FUNCTION SVM_Value_Interruption svm_kernel_get_interruption(const void* svm, const SVM_Kernel kernel);

/**
 * This function returns a string representation of the kernel.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel from which the state will be extracted.
 *
 * \note This API function can be useful in sequencer print callback functions.
 *
 * \returns A SVM_String containing a representation of the kernel.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 * 
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 */
SVM_FUNCTION SVM_String svm_kernel_print(const void* svm, const SVM_Kernel kernel);

/**
 * This function returns a string representation of the complete kernel, like in core dumps.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel from which the core dump  will be extracted.
 *
 * \note This API function can be useful for debugging purpose.
 *
 * \returns A SVM_String containing a core dump of the kernel.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 *
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 */
SVM_FUNCTION SVM_String svm_kernel_get_coredump(const void *svm, const SVM_Kernel kernel);

/** \} */

/** \defgroup executionkernels SVM kernel execution
 * This module contains API functions to handle SVM kernel execution.
 * \{
 */

/**
 * This function asks a kernel for suspension.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel to be suspended.
 *
 * \note The suspension of the kernel is asynchronous, and will be performed at the end of the execution of its current instruction.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION void svm_kernel_suspend(const void *svm, const SVM_Kernel kernel);

/**
 * This function asks a kernel for terminaison.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel to be terminated.
 *
 * \note The terminaison of the kernel is asynchronous, and will be performed at the end of the execution of its current instruction.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION void svm_kernel_terminate(const void *svm, const SVM_Kernel kernel);

/** \} */

/** \defgroup operationskernels SVM kernel operations
 * This module contains API functions to handle SVM kernel operations.
 * \{
 */

/**
 * This function swaps memories of two kernels.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in,out] kernel1 The first kernel of memory exchange.
 * \param[in,out] kernel2 The second kernel of memory exchange.
 *
 * This function swaps the memories of the two kernels provided in input, no matter their state.
 * This function also works on detached kernels.
 *
 * \note The two kernels have to be different kernels.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when at least one kernel is not owned by our process or is not in a process locked by ours.
 * \throws FAILURE interruption when the two kernels are the same kernel.
 */
SVM_FUNCTION void svm_kernel_swap_memory(const void *svm, SVM_Kernel kernel1, SVM_Kernel kernel2);

/** \} */

/** \} */

/** \defgroup processes SVM processes
 * This module contains API functions to handle SVM processes.
 *
 * A process is a dedicated execution thread executing kernels in a sequence.
 *
 * Processes are executed in parallel using schedulers to orchestrate their execution.
 *
 * \note A process is a common entry point in this API to get access to various elements linked to it.
 *
 * \see kernels
 * \see schedulers
 *
 * \{
 */

/** \defgroup getprocesses SVM process getters
 * This module contains API functions to get SVM processes.
 * \{
 */

/**
 * This function creates a new process from a kernel and the name from a string value.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] name The name of the process.
 * \param[in] sequencer The sequencer name. A NULL pointer will select the default sequencer.
 * \param[in] auto_terminated This flag indicates whether the process shall be automatically terminated.
 * \param[in] kernel The first kernel of the process.
 *
 * The process is created in SUSPENDED state and has to be attached to a scheduler to be executed.
 *
 * \note Several processes can have the same name. This name will be used for debugging purpose.
 *
 * \returns The created process.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Process svm_process_new(const void *svm, const SVM_Value_String name, const SVM_Value_PluginEntryPoint sequencer, const SVM_Boolean auto_terminated, const SVM_Kernel kernel);

/**
 * This function creates a new process from a kernel and the name from a SVM string.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] name The name of the process.
 * \param[in] sequencer The sequencer name. A NULL pointer will select the default sequencer.
 * \param[in] auto_terminated This flag indicates whether the process shall be automatically terminated.
 * \param[in] kernel The first kernel of the process.
 *
 * The process is created in SUSPENDED state and has to be attached to a scheduler to be executed.
 *
 * \note Several processes can have the same name. This name will be used for debugging purpose.
 *
 * \returns The created process.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Process svm_process_new__string(const void *svm, const SVM_String name, const SVM_Value_PluginEntryPoint sequencer, const SVM_Boolean auto_terminated, const SVM_Kernel kernel);

/**
 * This function creates a new process from a kernel and the name from a C nul-terminated string.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] name The name of the process.
 * \param[in] sequencer The sequencer name. A NULL pointer will select the default sequencer.
 * \param[in] auto_terminated This flag indicates whether the process shall be automatically terminated.
 * \param[in] kernel The first kernel of the process.
 *
 * The process is created in SUSPENDED state and has to be attached to a scheduler to be executed.
 *
 * \note Several processes can have the same name. This name will be used for debugging purpose.
 *
 * \returns The created process.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Process svm_process_new__raw(const void *svm, const char *name, const SVM_Value_PluginEntryPoint sequencer, const SVM_Boolean auto_terminated, const SVM_Kernel kernel);

/**
 * This function creates a new process from specific values and the name from a string value.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] name The name of the process.
 * \param[in] sequencer The sequencer name. A NULL pointer will select the default sequencer.
 * \param[in] auto_terminated This flag indicates whether the process shall be automatically terminated.
 * \param[in] code The SVM code to execute in the first kernel of the process.
 * \param[in] transmit_interruptions This flag asks the first kernel to forward the interruption to the next kernel or the process.
 * \param[in] last_return_is_shutdown This flag indicates whether an extra return on an empty return stack can be considered as a shutdown in the first kernel of the process.
 * \param[in] protected_mode The protected mode of the first kernel of the process.
 * \param[in] access_control The access control of the first kernel of the process. A NULL pointer means no specific restriction.
 *
 * The process is created in SUSPENDED state and has to be attached to a scheduler to be executed.
 *
 * \note When this API function is called within a kernel, the protected mode and the access control of the first kernel of the process are always set as restrictions of the current limitations.
 * In particular, if the current kernel executes code in protected mode, the created kernel within the process will automatically be in protected mode, no matter the protected_mode flag could be.
 *
 * \note Access control is ignored when the first kernel of the process is not created in protected mode.
 *
 * \warning When this API function is called outside a kernel, the protected mode and access control parameters are directly applied.
 * This can lead to a security issue unless either the plugin callback containing such call can only be triggered by a system instruction, or the code is coming only from safe sources.
 *
 * \note Several processes can have the same name. This name will be used for debugging purpose.
 *
 * \returns The created process.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Process svm_process_new_code(const void *svm, const SVM_Value_String name, const SVM_Value_PluginEntryPoint sequencer, const SVM_Boolean auto_terminated, const SVM_Code code, const SVM_Boolean transmit_interruptions, const SVM_Boolean last_return_is_shutdown, const SVM_Boolean protected_mode, const SVM_AccessControl access_control);

/**
 * This function creates a new process from specific values and the name from a SVM_String.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] name The name of the process.
 * \param[in] sequencer The sequencer name. A NULL pointer will select the default sequencer.
 * \param[in] auto_terminated This flag indicates whether the process shall be automatically terminated.
 * \param[in] code The SVM code to execute in the first kernel of the process.
 * \param[in] transmit_interruptions This flag asks the first kernel to forward the interruption to the next kernel or the process.
 * \param[in] last_return_is_shutdown This flag indicates whether an extra return on an empty return stack can be considered as a shutdown in the first kernel of the process.
 * \param[in] protected_mode The protected mode of the first kernel of the process.
 * \param[in] access_control The access control of the first kernel of the process. A NULL pointer means no specific restriction.
 *
 * The process is created in SUSPENDED state and has to be attached to a scheduler to be executed.
 *
 * \note When this API function is called within a kernel, the protected mode and the access control of the first kernel of the process are always set as restrictions of the current limitations.
 * In particular, if the current kernel executes code in protected mode, the created kernel within the process will automatically be in protected mode, no matter the protected_mode flag could be.
 *
 * \note Access control is ignored when the first kernel of the process is not created in protected mode.
 *
 * \warning When this API function is called outside a kernel, the protected mode and access control parameters are directly applied.
 * This can lead to a security issue unless either the plugin callback containing such call can only be triggered by a system instruction, or the code is coming only from safe sources.
 *
 * \note Several processes can have the same name. This name will be used for debugging purpose.
 *
 * \returns The created process.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Process svm_process_new_code__string(const void *svm, const SVM_String name, const SVM_Value_PluginEntryPoint sequencer, const SVM_Boolean auto_terminated, const SVM_Code code, const SVM_Boolean transmit_interruptions, const SVM_Boolean last_return_is_shutdown, const SVM_Boolean protected_mode, const SVM_AccessControl access_control);

/**
 * This function creates a new process from specific values and the name from a C nul-terminated string.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] name The name of the process.
 * \param[in] sequencer The sequencer name. A NULL pointer will select the default sequencer.
 * \param[in] auto_terminated This flag indicates whether the process shall be automatically terminated.
 * \param[in] code The SVM code to execute in the first kernel of the process.
 * \param[in] transmit_interruptions This flag asks the first kernel to forward the interruption to the next kernel or the process.
 * \param[in] last_return_is_shutdown This flag indicates whether an extra return on an empty return stack can be considered as a shutdown in the first kernel of the process.
 * \param[in] protected_mode The protected mode of the first kernel of the process.
 * \param[in] access_control The access control of the first kernel of the process. A NULL pointer means no specific restriction.
 *
 * The process is created in SUSPENDED state and has to be attached to a scheduler to be executed.
 *
 * \note When this API function is called within a kernel, the protected mode and the access control of the first kernel of the process are always set as restrictions of the current limitations.
 * In particular, if the current kernel executes code in protected mode, the created kernel within the process will automatically be in protected mode, no matter the protected_mode flag could be.
 *
 * \note Access control is ignored when the first kernel of the process is not created in protected mode.
 *
 * \warning When this API function is called outside a kernel, the protected mode and access control parameters are directly applied.
 * This can lead to a security issue unless either the plugin callback containing such call can only be triggered by a system instruction, or the code is coming only from safe sources.
 *
 * \note Several processes can have the same name. This name will be used for debugging purpose.
 *
 * \returns The created process.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Process svm_process_new_code__raw(const void *svm, const char *name, const SVM_Value_PluginEntryPoint sequencer, const SVM_Boolean auto_terminated, const SVM_Code code, const SVM_Boolean transmit_interruptions, const SVM_Boolean last_return_is_shutdown, const SVM_Boolean protected_mode, const SVM_AccessControl access_control);

/**
 * This function creates a new process from specific values and the name from a string value.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] name The name of the process.
 * \param[in] sequencer The sequencer name. A NULL pointer will select the default sequencer.
 * \param[in] auto_terminated This flag indicates whether the process shall be automatically terminated.
 * \param[in] symbol The symbol to jump to in the first kernel of the process.
 * \param[in] transmit_interruptions This flag asks the first kernel to forward the interruption to the next kernel or the process.
 * \param[in] last_return_is_shutdown This flag indicates whether an extra return on an empty return stack can be considered as a shutdown in the first kernel of the process.
 * \param[in] protected_mode The protected mode of the first kernel of the process.
 * \param[in] access_control The access control of the first kernel of the process. A NULL pointer means no specific restriction.
 *
 * The process is created in SUSPENDED state and has to be attached to a scheduler to be executed.
 *
 * \note When this API function is called within a kernel, the protected mode and the access control of the first kernel of the process are always set as restrictions of the current limitations.
 * In particular, if the current kernel executes code in protected mode, the created kernel within the process will automatically be in protected mode, no matter the protected_mode flag could be.
 *
 * \note Access control is ignored when the first kernel of the process is not created in protected mode.
 *
 * \warning When this API function is called outside a kernel, the protected mode and access control parameters are directly applied.
 * This can lead to a security issue unless either the plugin callback containing such call can only be triggered by a system instruction, or the code is coming only from safe sources.
 *
 * \note Several processes can have the same name. This name will be used for debugging purpose.
 *
 * \returns The created process.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Process svm_process_new_symbol(const void *svm, const SVM_Value_String name, const SVM_Value_PluginEntryPoint sequencer, const SVM_Boolean auto_terminated, const SVM_Value_Symbol symbol, const SVM_Boolean transmit_interruptions, const SVM_Boolean last_return_is_shutdown, const SVM_Boolean protected_mode, const SVM_AccessControl access_control);

/**
 * This function creates a new process from specific values and the name from a SVM_String.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] name The name of the process.
 * \param[in] sequencer The sequencer name. A NULL pointer will select the default sequencer.
 * \param[in] auto_terminated This flag indicates whether the process shall be automatically terminated.
 * \param[in] symbol The symbol to jump to in the first kernel of the process.
 * \param[in] transmit_interruptions This flag asks the first kernel to forward the interruption to the next kernel or the process.
 * \param[in] last_return_is_shutdown This flag indicates whether an extra return on an empty return stack can be considered as a shutdown in the first kernel of the process.
 * \param[in] protected_mode The protected mode of the first kernel of the process.
 * \param[in] access_control The access control of the first kernel of the process. A NULL pointer means no specific restriction.
 *
 * The process is created in SUSPENDED state and has to be attached to a scheduler to be executed.
 *
 * \note When this API function is called within a kernel, the protected mode and the access control of the first kernel of the process are always set as restrictions of the current limitations.
 * In particular, if the current kernel executes code in protected mode, the created kernel within the process will automatically be in protected mode, no matter the protected_mode flag could be.
 *
 * \note Access control is ignored when the first kernel of the process is not created in protected mode.
 *
 * \warning When this API function is called outside a kernel, the protected mode and access control parameters are directly applied.
 * This can lead to a security issue unless either the plugin callback containing such call can only be triggered by a system instruction, or the code is coming only from safe sources.
 *
 * \note Several processes can have the same name. This name will be used for debugging purpose.
 *
 * \returns The created process.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Process svm_process_new_symbol__string(const void *svm, const SVM_String name, const SVM_Value_PluginEntryPoint sequencer, const SVM_Boolean auto_terminated, const SVM_Value_Symbol symbol, const SVM_Boolean transmit_interruptions, const SVM_Boolean last_return_is_shutdown, const SVM_Boolean protected_mode, const SVM_AccessControl access_control);

/**
 * This function creates a new process from specific values and the name from a C nul-terminated string.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] name The name of the process.
 * \param[in] sequencer The sequencer name. A NULL pointer will select the default sequencer.
 * \param[in] auto_terminated This flag indicates whether the process shall be automatically terminated.
 * \param[in] symbol The symbol to jump to in the first kernel of the process.
 * \param[in] transmit_interruptions This flag asks the first kernel to forward the interruption to the next kernel or the process.
 * \param[in] last_return_is_shutdown This flag indicates whether an extra return on an empty return stack can be considered as a shutdown in the first kernel of the process.
 * \param[in] protected_mode The protected mode of the first kernel of the process.
 * \param[in] access_control The access control of the first kernel of the process. A NULL pointer means no specific restriction.
 *
 * The process is created in SUSPENDED state and has to be attached to a scheduler to be executed.
 *
 * \note When this API function is called within a kernel, the protected mode and the access control of the first kernel of the process are always set as restrictions of the current limitations.
 * In particular, if the current kernel executes code in protected mode, the created kernel within the process will automatically be in protected mode, no matter the protected_mode flag could be.
 *
 * \note Access control is ignored when the first kernel of the process is not created in protected mode.
 *
 * \warning When this API function is called outside a kernel, the protected mode and access control parameters are directly applied.
 * This can lead to a security issue unless either the plugin callback containing such call can only be triggered by a system instruction, or the code is coming only from safe sources.
 *
 * \note Several processes can have the same name. This name will be used for debugging purpose.
 *
 * \returns The created process.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Process svm_process_new_symbol__raw(const void *svm, const char *name, const SVM_Value_PluginEntryPoint sequencer, const SVM_Boolean auto_terminated, const SVM_Value_Symbol symbol, const SVM_Boolean transmit_interruptions, const SVM_Boolean last_return_is_shutdown, const SVM_Boolean protected_mode, const SVM_AccessControl access_control);

/**
 * This function retrieves the current process.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 *
 * \warning The resulting variable can be set global and added to a C/C++ structure linked to a type which can be written into the memory of the current kernel. In such case, a circular loop is created, preventing the variable to be destroyed at the proper time unless the variable is set local outside the type destructor or the value is explicitely removed from the kernel memory.
 *
 * \returns The current process if the API function is invoked inside a process, a NULL pointer otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Process svm_process_get_current(const void *svm);

/** \} */

/** \defgroup ownershipprocesses SVM process ownership
 * This module contains API functions to handle SVM process ownership.
 * \{
 */

/**
 * This function locks a process from the current process.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] process The process to lock.
 *
 * This function waits until the lock is acquired by the current process, unless an invalid lock attempt is performed.
 *
 * The returned lock is a recursive lock: A lock attempt on a process already locked by the current process is accepted.
 * All locks on the remote process have to be released to unlock the process.
 *
 * \note To release the remote process, the returned lock has to be freed. See svm_variable_delete API function for more details.
 *
 * A lock will never be acquired on:
 * - the current process,
 * - a dead process,
 * - a process creating a circular dead lock.
 *
 * \returns The process lock if the process is owned by the current process, or a NULL pointer otherwise.
 *
 * \warning If a NULL pointer is returned by this function, any action on the process can lead to an undefined behavior.
 *
 * Usage example:
 * \code{.c}
 * SVM_Process_Lock lock = svm_process_ownership_lock(svm,process);
 * if(svm_process_ownership_check(svm,process))
 * {
 * 	// current process owns process: operations on process allowed
 * }
 * if(lock)
 * {
 * 	// lock retains process execution
 * 	// lock variable can now be safely set as global and/or shared, and stored
 * 	// lock variable can be freed to release process execution
 * }
 * \endcode
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when this API function is called outside a waiting instruction.
 *
 * \see svm_variable_delete
 * \see svm_process_ownership_get_local
 * \see svm_process_ownership_check
 */
SVM_FUNCTION SVM_Process_Lock svm_process_ownership_lock(const void *svm, const SVM_Process process);

/**
 * This function locks all other processes.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 *
 * This function locks all other processes for critical execution of code.
 *
 * \warning Use this function with caution, as it can block the virtual machine. It is also avised to limit the lock scope to the instruction creating the lock.
 *
 * \note To release the all processes, the returned lock has to be freed. See svm_variable_delete API function for more details.
 *
 * \returns The process lock if all other processes are locked by us, or a NULL pointer otherwise.
 *
 * \warning If a NULL pointer is returned by this function, any critical action performed can lead to an undefined behavior.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when this API function is called outside a system waiting instruction.
 *
 * \see svm_variable_delete
 * \see svm_process_ownership_get_local
 * \see svm_process_ownership_check
 */
SVM_FUNCTION SVM_Process_Lock svm_process_ownership_lock_critical(const void *svm);

/**
 * This function checks whether the current process owns another process.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] process The process ownership to check.
 *
 * The current process owns another process when:
 * - Either the process is also the current process,
 * - Or a lock exists on the process from the current process.
 *
 * \returns TRUE when the current process owns the process, FALSE otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 *
 * \see svm_process_ownership_lock
 * \see svm_process_ownership_lock_critical
 * \see svm_variable_delete
 */
SVM_FUNCTION SVM_Boolean svm_process_ownership_check(const void *svm, const SVM_Process process);

/**
 * This function locally takes ownership over another process.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] process The process ownership to take locally.
 *
 * The current process takes the ownership of the process until the end of the plugin callback.
 *
 * \note If a lock is acquired, it is placed in an hidden local variable.
 *
 * \returns TRUE when the current process succeeds to own the process, FALSE otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when this API function is called outside a waiting instruction.
 *
 * \see svm_process_ownership_lock
 * \see svm_process_ownership_lock_critical
 * \see svm_variable_delete
 */
SVM_FUNCTION SVM_Boolean svm_process_ownership_get_local(const void *svm, const SVM_Process process);

/** \} */

/** \defgroup executeprocesses SVM process execution
 * This module contains API functions to handle SVM process execution.
 * \{
 */

/**
 * This function requires the execution of a process with a delay as an integer value.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] process The process to run.
 * \param[in] delay The delay before the process replies when locked.
 *
 * The process does need to be locked by us.
 *
 * \note The execution of the process is asynchronous, and it will start run once the execution request reached the process.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when this API function is called from a process.
 */
SVM_FUNCTION void svm_process_run(const void *svm, const SVM_Process process, const SVM_Value_Integer delay);

/**
 * This function requires the execution of a process with a delay as an raw integer.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] process The process to run.
 * \param[in] delay The delay before the process replies when locked.
 *
 * The process does need to be locked by us.
 *
 * \note The execution of the process is asynchronous, and it will start run once the execution request reaches the process.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when this API function is called from a process.
 */
SVM_FUNCTION void svm_process_run__raw(const void *svm, const SVM_Process process, const unsigned long int delay);

/**
 * This function requires the suspension of a process.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] process The process to suspend.
 *
 * The process does need to be locked by us.
 *
 * \note The suspension of the process is asynchronous, and it will be suspension once the suspension request reaches the process.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION void svm_process_suspend(const void *svm, const SVM_Process process);

/**
 * This function requires the terminaison of a process.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] process The process to terminate.
 *
 * The process does need to be locked by us.
 *
 * \note The terminaison of the process is asynchronous, and it will be terminated once the terminaison request reaches the process.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION void svm_process_terminate(const void *svm, const SVM_Process process);

/**
 * This function changes the process status to notify it enters in a blocking wait inside an instruction.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] process The process entering the blocking wait.
 *
 * The process status is changed to WAITING and a notification is sent to its scheduler.
 *
 * \note If the instruction execution is reached before a call to svm_process_resume, the process is automatically resumed before the next instruction execution.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the process is not ours or is not a process locked by ours.
 * \throws FAILURE interruption when this API function is called outside a waiting instruction.
 *
 * \see svm_process_ownership_lock
 * \see svm_process_get_current
 */
SVM_FUNCTION void svm_process_pause(const void *svm, const SVM_Process process);

/**
 * This function changes the process status to notify it exits a blocking wait inside an instruction.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] process The process exiting the blocking wait.
 *
 * The process status is changed to SUSPENDED and a notification is sent to its scheduler.
 * The execution restart will be triggered by the scheduler.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the process is not ours or is not a process locked by ours.
 * \throws FAILURE interruption when this API function is called outside a waiting instruction.
 *
 * \see svm_process_ownership_lock
 * \see svm_process_get_current
 */
SVM_FUNCTION void svm_process_resume(const void *svm, const SVM_Process process);

/** \} */

/** \defgroup notifyprocesses SVM process notifications
 * This module contains API functions to handle SVM process interruption notifications.
 * \{
 */

/**
 * This function changes the process behavior on interruption event.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] process The process allowing to be notified about interruption events.
 *
 * The process will accept notification on hardware interruptions and scheduling events like suspension, termination and lock.
 * On interruption event, the process will receive a technical signal associated to an empty handler to unlock execution from blocking syscalls.
 * This signal is not related to the eventual hardware interruption signal triggering this notification.
 *
 * \note The interruption notifications are received even if no hardware interruption handlers are defined for the received hardware interruptions.
 *
 * \warning This feature needs to be activated at SVM compilation time (Please refer to the configure --enable-linux-extensions option) and may not work.
 *
 * Usage example:
 * \code{.c}
 * char buffer[1024];
 * svm_process_interruptionnotification_enable(svm,svm_process_get_current(svm));
 * ssize_t s = read(fd,buffer,1024);
 * svm_process_interruptionnotification_disable(svm,svm_process_get_current(svm));
 * if(s<0)
 * {
 * 	if(errno==EINTR)
 * 	{
 * 		// an interruption event has been received by the SVM.
 * 		return svm_value_string_new_null(svm);
 * 	}
 *	// other error handling here
 * }
 * return svm_value_string_new__buffer(svm,buffer,s);
 * \endcode
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the process is not ours or is not a process locked by ours.
 * \throws FAILURE interruption when this API function is called outside a waiting instruction.
 *
 * \see svm_process_ownership_lock
 * \see svm_process_get_current
 */
SVM_FUNCTION void svm_process_interruptionnotification_enable(const void *svm, const SVM_Process process);

/**
 * This function changes the process behavior on interruption event.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] process The process allowing to be notified about interruption events.
 *
 * The process will ignore interruption event notifications.
 * On interruption event reception, any blocking syscall will continue its execution.
 *
 * \warning This feature needs to be activated at SVM compilation time (Please refer to the configure --enable-linux-extensions option) and may not work.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the process is not ours or is not a process locked by ours.
 * \throws FAILURE interruption when this API function is called outside a waiting instruction.
 *
 * \see svm_process_ownership_lock
 * \see svm_process_get_current
 */
SVM_FUNCTION void svm_process_interruptionnotification_disable(const void *svm, const SVM_Process process);

/**
 * This function waits until the current process receives an interruption notification.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 *
 * To distinguish between an interruption from the scheduler or an hardware interruption, use the svm_processor_is_hardwareinterrupted API function.
 *
 * \note The API function temporarly enables the interruption notifications.
 *
 * \warning This feature needs to be activated at SVM compilation time (Please refer to the configure --enable-linux-extensions option) and may not work.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when called outside a process.
 * \throws FAILURE interruption when this API function is called outside a waiting instruction.
 *
 * \see svm_processor_is_hardwareinterrupted
 */
SVM_FUNCTION void svm_process_interruptionnotification_wait(const void *svm);

/**
 * This function sends an interruption notification to a process.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] process The process to notify.
 *
 * The process will be notified only if it accepts interruption notifications.
 *
 * \warning This feature needs to be activated at SVM compilation time (Please refer to the configure --enable-linux-extensions option) and may not work.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 *
 * \see svm_process_interruptionnotification_enable
 * \see svm_process_interruptionnotification_wait
 */
SVM_FUNCTION void svm_process_interruptionnotification_send(const void *svm, const SVM_Process process);

/** \} */

/** \defgroup statusprocesses SVM process status
 * This module contains API functions to handle SVM process status.
 * \{
 */

/**
 * This function retrieves the name of a process.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] process The process from which the status will be extracted.
 *
 * \returns The process name.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_String svm_process_get_name(const void *svm, const SVM_Process process);

/**
 * This function retrieves the status of a process.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] process The process from which the status will be extracted.
 *
 * \returns The process status.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Process_State svm_process_get_state(const void *svm, const SVM_Process process);

/**
 * This function retrieves the interruption of an interrupted process.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] process The process from which the interruption will be extracted.
 *
 * The process does need to be locked by us.
 *
 * \returns If the process is interrupted, the interruption of the process, a NULL pointer otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Value_Interruption svm_process_get_interruption(const void* svm, const SVM_Process process);

/**
 * This function returns a minimal string representation of the process state.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] process The process from which the status will be extracted.
 *
 * The process does need to be locked by us.
 *
 * \returns A SVM_String containing a minimal representation of the process state.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_String svm_process_print(const void *svm, const SVM_Process process);

/**
 * This function returns a string representation of the process.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] process The process from which the status will be extracted.
 *
 * The process does need to be locked by us.
 *
 * \returns A SVM_String containing a representation of the process.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_String svm_process_get_coredump(const void *svm, const SVM_Process process);

/** \} */

/** \defgroup schedulerprocesses SVM process scheduler
 * This module contains API functions to handle SVM process scheduler.
 * \{
 */

/**
 * This function checks whether a process is attached to a scheduler.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] process The process from which the scheduler will be extracted.
 *
 * \returns TRUE if the process is attached to a scheduler, FALSE otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Boolean svm_process_has_scheduler(const void *svm, const SVM_Process process);

/**
 * This function returns the name of the scheduler of a process.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] process The process from which the scheduler will be extracted.
 *
 * \note If the process uses the default scheduler, a NULL pointer is returned.
 *
 * \returns The name of the scheduler.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the process is not attached to a scheduler.
 */
SVM_FUNCTION SVM_Value_PluginEntryPoint svm_process_get_scheduler(const void *svm, const SVM_Process process);

/** \} */

/** \defgroup sequencerprocesses SVM process sequencer
 * This module contains API functions to handle SVM process sequencers.
 * \{
 */

/**
 * This function returns the name of the sequencer of a process.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] process The process from which the sequencer will be extracted.
 *
 * \note If the process uses the default sequencer, a NULL pointer is returned.
 *
 * \returns The name of the sequencer.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Value_PluginEntryPoint svm_process_get_sequencer(const void *svm, const SVM_Process process);

/**
 * This function returns the internal structure pointer of the sequencer of a process.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] process The process from which the sequencer will be extracted.
 *
 * \note The returned value does not need to be freed/deleted, as it is managed by the sequencer of the process.
 *
 * \note If the process uses the default sequencer, a NULL pointer is returned.
 *
 * \note When this API function is used on a sequencer, synchronisation mechanisms have to be put in place to protect concurrent access to this structure.
 *
 * \returns The internal structure of the sequencer.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION void* svm_process_sequencer_get_internal(const void *svm, const SVM_Process process);

/**
 * This function produces a string representation of the sequencer of a process.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] process The process from which the sequencer will be printed.
 *
 * \returns The string representation of the sequencer.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_String svm_process_sequencer_print(const void *svm, const SVM_Process process);

/** \} */

/** \defgroup kernelprocesses SVM process kernels
 * This module contains API functions to handle SVM process kernels.
 * \{
 */

/**
 * This function attaches a kernel to a process.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in,out] process The receiving process.
 * \param[in] kernel The kernel to attach.
 * \param[in] argc The number of parameters.
 * \param[in] argv The parameters.
 *
 * The parameters are sent verbatim to the sequencer attach plugin callback.
 * The argv parameter can be a NULL pointer if the sequencer callback accepts it.
 *
 * \warning If the argv is not a NULL pointer and its size is smaller than argc, it will result in an undefined behavior.
 *
 * \returns TRUE if the kernel is attached to a process, FALSE otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel can not be attached to the process.
 * \throws FAILURE interruption when the process is not ours or is not a process locked by ours.
 *
 * \see svm_process_ownership_lock
 * \see svm_process_get_current
 */
SVM_FUNCTION SVM_Boolean svm_process_kernel_attach(const void *svm, SVM_Process process, const SVM_Kernel kernel, const SVM_Size argc, SVM_Parameter argv[]);

/**
 * This function detaches a kernel from a process.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in,out] process The giving process.
 * \param[in] kernel The kernel to detach.
 * \param[in] argc The number of parameters.
 * \param[in] argv The parameters.
 *
 * The parameters are sent verbatim to the sequencer detach plugin callback.
 * The argv parameter can be a NULL pointer if the sequencer callback accepts it.
 *
 * \warning If the argv is not a NULL pointer and its size is smaller than argc, it will result in an undefined behavior.
 *
 * \returns TRUE if the kernel is detached from a process, FALSE otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel can not be detached from the process.
 * \throws FAILURE interruption when the process is not ours or is not a process locked by ours.
 *
 * \see svm_process_ownership_lock
 * \see svm_process_get_current
 */
SVM_FUNCTION SVM_Boolean svm_process_kernel_detach(const void *svm, SVM_Process process, const SVM_Kernel kernel, const SVM_Size argc, SVM_Parameter argv[]);

/**
 * This function retrieves the kernel currently executed on a process.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] process The process from which the current kernel will be extracted.
 *
 * \returns The current kernel executed in a process or a NULL pointer when the process is a zombie.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the process is not ours or is not a process locked by ours.
 *
 * \see svm_process_ownership_lock
 * \see svm_process_get_current
 */
SVM_FUNCTION SVM_Kernel svm_process_kernel_get_current(const void *svm, const SVM_Process process);

/** \} */

/** \} */

/** \defgroup schedulers SVM schedulers
 * This module contains API functions to handle SVM schedulers.
 *
 * A scheduler is a dedicated orchestrator thread controlling process execution.
 *
 * Schedulers work by reacting to some events they receive from processes.
 *
 * \note Schedulers do not manage the execution sequence of kernels within a process.
 * This specific orchestration is managed by sequencers.
 *
 * \see processes
 * \see sequencerprocesses
 *
 * \{
 */

/**
 * This function retrieves a scheduler from its name.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] name The scheduler name.
 *
 * \note If parameter name is NULL, the default scheduler is returned.
 *
 * \returns The scheduler.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the name is not associated to a scheduler.
 */
SVM_FUNCTION SVM_Scheduler svm_scheduler_get(const void *svm, const SVM_Value_PluginEntryPoint name);

/**
 * This function retrieves the current scheduler.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 *
 * \returns The current scheduler, or a NULL pointer when outside a scheduler context.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Scheduler svm_scheduler_get_current(const void *svm);

/**
 * This function extracts the internal structure of a scheduler.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] scheduler The scheduler from which the internal structure is extracted.
 *
 * \note When this API function is used on a scheduler, synchronisation mechanisms have to be put in place to protect concurrent access to this structure.
 *
 * \returns The internal structure pointer.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION void* svm_scheduler_get_internal(const void *svm, const SVM_Scheduler scheduler);

/**
 * This function returns a string representation of the state of a scheduler.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] scheduler The scheduler from which the status will be extracted.
 *
 * \returns A SVM_String containing a representation of the scheduler state.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_String svm_scheduler_print(const void *svm, const SVM_Scheduler scheduler);

/**
 * This function attaches a process to a scheduler with an integer value parameter.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] scheduler The receiving scheduler.
 * \param[in] process The process to be attached.
 * \param[in] parameter A parameter.
 *
 * The parameter is passed to the scheduler plugin callback attaching the process.
 *
 * \note When the attach operation fails on a newly created process and the process variable is kept local, the process remains in the system. This dangling process should be unlocked then terminated.
 *
 * \returns TRUE if the process has been attached, FALSE otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the process is not ours or is not a process locked by ours.
 * \throws FAILURE interruption when the API function is called outside a process.
 * 
 * \see svm_process_ownership_lock
 * \see svm_process_get_current
 * \see svm_process_terminate
 * \see svm_scheduler_get
 */
SVM_FUNCTION SVM_Boolean svm_scheduler_process_attach(const void *svm, const SVM_Scheduler scheduler, const SVM_Process process, const SVM_Value_Integer parameter);

/**
 * This function attaches a process to a scheduler with a raw integer parameter.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] scheduler The receiving scheduler.
 * \param[in] process The process to be attached.
 * \param[in] parameter A parameter.
 *
 * The parameter is passed to the scheduler plugin callback attaching the process.
 *
 * \note When the attach operation fails on a newly created process and the process variable is kept local, the process remains in the system. This dangling process should be unlocked then terminated.
 *
 * \returns TRUE if the process has been attached, FALSE otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the process is not ours or is not a process locked by ours.
 * \throws FAILURE interruption when the API function is called outside a process.
 *
 * \see svm_process_ownership_lock
 * \see svm_process_get_current
 * \see svm_process_terminate
 * \see svm_scheduler_get
 */
SVM_FUNCTION SVM_Boolean svm_scheduler_process_attach__raw(const void *svm, const SVM_Scheduler scheduler, const SVM_Process process, const unsigned long int parameter);

/**
 * This function detaches a process from a scheduler with an integer value parameter.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] scheduler The giving scheduler.
 * \param[in] process The process to be detached.
 * \param[in] parameter A parameter.
 *
 * The parameter is passed to the scheduler plugin callback detaching the process.
 *
 * \note When the detach operation succeeds on a process and the process variable is kept local, the process remains in the system. This dangling process should be unlocked then terminated.
 *
 * \returns TRUE if the process has been detached, FALSE otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the process is not ours or is not a process locked by ours.
 * \throws FAILURE interruption when the API function is called outside a process.
 *
 * \see svm_process_ownership_lock
 * \see svm_process_get_current
 * \see svm_process_terminate
 * \see svm_scheduler_get
 */
SVM_FUNCTION SVM_Boolean svm_scheduler_process_detach(const void *svm, const SVM_Scheduler scheduler, const SVM_Process process, const SVM_Value_Integer parameter);

/**
 * This function detaches a process from a scheduler with a raw integer parameter.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] scheduler The giving scheduler.
 * \param[in] process The process to be detached.
 * \param[in] parameter A parameter.
 *
 * The parameter is passed to the scheduler plugin callback detaching the process.
 *
 * \note When the detach operation succeeds on a process and the process variable is kept local, the process remains in the system. This dangling process should be unlocked then terminated.
 *
 * \returns TRUE if the process has been detached, FALSE otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the process is not ours or is not a process locked by ours.
 * \throws FAILURE interruption when the API function is called outside a process.
 *
 * \see svm_process_ownership_lock
 * \see svm_process_get_current
 * \see svm_process_terminate
 * \see svm_scheduler_get
 */
SVM_FUNCTION SVM_Boolean svm_scheduler_process_detach__raw(const void *svm, const SVM_Scheduler scheduler, const SVM_Process process, const unsigned long int parameter);

/**
 * This function sends a notification to a scheduler with an integer value parameter.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] scheduler The scheduler to notify.
 * \param[in] parameter A parameter.
 *
 * \note If the scheduler does not support notifications, the notification is silently discarded.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION void svm_scheduler_notify(const void *svm, const SVM_Scheduler scheduler, const SVM_Value_Integer parameter);

/**
 * This function sends a notification to a scheduler with a raw integer parameter.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] scheduler The scheduler to notify.
 * \param[in] parameter A parameter.
 *
 * \note If the scheduler does not support notifications, the notification is silently discarded.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION void svm_scheduler_notify__raw(const void *svm, const SVM_Scheduler scheduler, const unsigned long int parameter);

/** \} */

/** \defgroup synchronisation SVM synchronisation
 * This module contains API functions to handle SVM synchronisation.
 *
 * Synchronisation mechanisms are useful when several processes have to either access to shared ressources or work in concurrence.
 *
 * Basic synchronisation tools are provided here with integration in a multi-process environnement.
 *
 * \{
 */

/** \defgroup lockssynchronisation SVM synchronisation locks
 * This module contains API functions to handle SVM synchronisation locks.
 * \{
 */

/**
 * This function creates a lock.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 *
 * This lock supports the mutual exclusion of one writer and several concurent readers.
 *
 * \note The lock will be kept alive while either a variable or a read lock guard or a write lock guard exists on this lock.
 *
 * \returns The lock. This lock is not acquired here.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Lock svm_lock_new(const void *svm);

/**
 * This function locks a lock for read access.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] lock The lock to acquire.
 * \param[in] soft The process is put in pause while waiting for the lock acquisition.
 *
 * The lock is acquired for read access until the returned lock guard is freed/deleted. Please refer to the svm_variable_delete API function.
 *
 * \note In protected mode, this function enforces a process pause and resume around the lock acquisition, if not done using API functions.
 *
 * \returns A read lock guard on the acquired lock.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when this API function is called outside a kernel.
 * \throws FAILURE interruption when this API function is called outside a waiting instruction.
 *
 * \see svm_variable_delete
 * \see svm_kernel_get_current
 */
SVM_FUNCTION SVM_LockGuard_Read svm_lock_readguard_new(const void *svm, const SVM_Lock lock, const SVM_Boolean soft);

/**
 * This function locks a lock for write access.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] lock The lock to acquire.
 * \param[in] soft The process is put in pause while waiting for the lock acquisition.
 *
 * The lock is acquired for write access until the returned lock guard is freed/deleted. Please refer to the svm_variable_delete API function.
 *
 * \note In protected mode, this function enforces a process pause and resume around the lock acquisition, if not done using API functions.
 *
 * \returns A write lock guard on the acquired lock.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when this API function is called outside a kernel.
 * \throws FAILURE interruption when this API function is called outside a waiting instruction.
 *
 * \see svm_variable_delete
 * \see svm_kernel_get_current
 */
SVM_FUNCTION SVM_LockGuard_Write svm_lock_writeguard_new(const void *svm, const SVM_Lock lock, const SVM_Boolean soft);

/** \} */

/** \defgroup eventssynchronization SVM synchronisation events
 * This module contains API functions to handle SVM synchronisation events.
 * \{
 */

/**
 * This function creates an event queue.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 *
 * This event queue allows synchronisation of a lot of concurrent processes by pushing events to this queue and pulling events from this queue.
 *
 * Each process accessing the queue will be identified by an address linked to a structure.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Event_Queue svm_event_queue_new(const void *svm);

/**
 * This function creates an event queue address.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 *
 * The address is created without association to an owner.
 * 
 * \returns The event queue address.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Event_Queue_Address svm_event_address_new(const void *svm);

/**
 * This function creates an event queue address.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] structure The structure owner of the address.
 *
 * The address is created with an association to an owner.
 * Creating an address with a reference to its owner allows an event puller to reach the pusher of the event.
 *
 * \note The owner structure variable does not need to be set global.
 * 
 * \returns The event queue address.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 *
 * \see svm_structure_new
 */
SVM_FUNCTION SVM_Event_Queue_Address svm_event_address_new_struct(const void *svm, const SVM_Structure structure);

/**
 * This function checks whether an event queue address has an owner.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] address The event queue address.
 *
 * \returns TRUE when the address has an owner, FALSE otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 *
 * \see svm_structure_get_internal
 */
SVM_FUNCTION SVM_Boolean svm_event_address_has_struct(const void *svm, const SVM_Event_Queue_Address address);

/**
 * This function retrieves the owner of an event queue address.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] address The event queue address.
 *
 * The owner of the address is returned.
 * If the address was not associated to an owner, a FAILURE interruption is raised.
 * 
 * \returns The owner of the event queue address.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the address has no owner.
 *
 * \see svm_structure_get_internal
 * \see svm_event_address_has_struct
 */
SVM_FUNCTION SVM_Structure svm_event_address_get_struct(const void *svm, const SVM_Event_Queue_Address address);

/**
 * This function lets an address to join the event queue.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in,out] event_queue The event queue on which the address will be added.
 * \param[in] address The address.
 *
 * This step is done by the owner of the address. It is required to push events to this address and to pull events from this address.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION void svm_event_queue_join(const void *svm, SVM_Event_Queue event_queue, const SVM_Event_Queue_Address address);

/**
 * This function lets an address to leave the event queue.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in,out] event_queue The event queue on which the address will be removed.
 * \param[in] address The address.
 *
 * This step is done by the owner of the address to close its participation to the event queue.
 *
 * \note All pending events are discarded.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION void svm_event_queue_leave(const void *svm, SVM_Event_Queue event_queue, const SVM_Event_Queue_Address address);

/**
 * This function pushes an event on a queue from an address to another address.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in,out] event_queue The event queue on which the event will be pushed.
 * \param[in] destination The address to which the event will be sent.
 * \param[in] origin The address from which the event is sent.
 * \param[in] event The structure containing the event.
 *
 * The event is pushed to the event queue.
 *
 * \note The event structure variable does not need to be set global.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the origin or the destination address has not joined the event queue.
 *
 * \see svm_structure_new
 */
SVM_FUNCTION void svm_event_queue_push(const void *svm, SVM_Event_Queue event_queue, const SVM_Event_Queue_Address destination, const SVM_Event_Queue_Address origin, const SVM_Structure event);

/**
 * This function pushes an event on a queue from an address to all addresses.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in,out] event_queue The event queue on which the event will be pushed.
 * \param[in] origin The address from which the event is sent.
 * \param[in] event The structure containing the event.
 *
 * The event is pushed to the event queue.
 *
 * \note The event structure variable does not need to be set global.
 *
 * \warning The structure will be shared between all addresses.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the origin address has not joined the event queue.
 *
 * \see svm_structure_new
 */
SVM_FUNCTION void svm_event_queue_broadcast(const void *svm, SVM_Event_Queue event_queue, const SVM_Event_Queue_Address origin, const SVM_Structure event);

/**
 * This function pulls an event on a queue from an address.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in,out] event_queue The event queue on which the event will be pushed.
 * \param[in] destination The address to which the event will be sent.
 * \param[out] origin The address from which the event is sent.
 * \param[out] event The structure containing the event.
 * \param[in] soft The process is put in pause while waiting for an event.
 *
 * The API function waits until an event is ready to be pulled.
 * When this API function is called within a process accepting interruption notifications, any interruption will stop the event pulling without a delivered message.
 *
 * \note In protected mode, this function enforces a process pause and resume around the event wait, if not done using API functions.
 *
 * \warning Accessing to origin and event when the API function has returned FALSE will result in an undefined behavior.
 *
 * \returns TRUE when an event has been delivered to the destination address, FALSE otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the destination address has not joined the event queue.
 * \throws FAILURE interruption when this API function is called outside a kernel.
 *
 * \see svm_structure_get_internal
 * \see svm_process_interruptionnotification_enable
 */
SVM_FUNCTION SVM_Boolean svm_event_queue_pull(const void *svm, SVM_Event_Queue event_queue, const SVM_Event_Queue_Address destination, SVM_Event_Queue_Address *origin, SVM_Structure *event, const SVM_Boolean soft);

/**
 * This function pulls an event on a queue from an address with a timeout in an integer value.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in,out] event_queue The event queue on which the event will be pushed.
 * \param[in] destination The address to which the event will be sent.
 * \param[out] origin The address from which the event is sent.
 * \param[out] event The structure containing the event.
 * \param[in] timeout A timeout in milliseconds.
 * \param[in] soft The process is put in pause while waiting for an event.
 *
 * The API function waits until an event is ready to be pulled or the timeout is reached.
 * A timeout of 0 exists immediately if no event is ready to be pulled.
 * When this API function is called within a process accepting interruption notifications, any interruption will stop the event pulling without a delivered message.
 *
 * \note In protected mode, this function enforces a process pause and resume around the event wait, if not done using API functions.
 *
 * \warning Accessing to origin and event when the API function has returned FALSE will result in an undefined behavior.
 *
 * \returns TRUE when an event has been delivered to the destination address, FALSE otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the destination address has not joined the event queue.
 * \throws FAILURE interruption when the timeout is a negative integer.
 * \throws FAILURE interruption when this API function is called outside a kernel.
 *
 * \see svm_structure_get_internal
 * \see svm_process_interruptionnotification_enable
 */
SVM_FUNCTION SVM_Boolean svm_event_queue_check(const void *svm, SVM_Event_Queue event_queue, const SVM_Event_Queue_Address destination, SVM_Event_Queue_Address *origin, SVM_Structure *event, const SVM_Value_Integer timeout, const SVM_Boolean soft);

/**
 * This function pulls an event on a queue from an address with a timeout in a raw integer.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in,out] event_queue The event queue on which the event will be pushed.
 * \param[in] destination The address to which the event will be sent.
 * \param[out] origin The address from which the event is sent.
 * \param[out] event The structure containing the event.
 * \param[in] timeout A timeout in milliseconds.
 * \param[in] soft The process is put in pause while waiting for an event.
 *
 * The API function waits until an event is ready to be pulled or the timeout is reached.
 * A timeout of 0 exists immediately if no event is ready to be pulled.
 *
 * \note In protected mode, this function enforces a process pause and resume around the event wait, if not done using API functions.
 *
 * \warning Accessing to origin and event when the API function has returned FALSE will result in an undefined behavior.
 *
 * \returns TRUE when an event has been delivered to the destination address, FALSE otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the destination address has not joined the event queue.
 * \throws FAILURE interruption when the timeout is a negative integer.
 * \throws FAILURE interruption when this API function is called outside a kernel.
 *
 * \see svm_structure_get_internal
 */
SVM_FUNCTION SVM_Boolean svm_event_queue_check__raw(const void *svm, SVM_Event_Queue event_queue, const SVM_Event_Queue_Address destination, SVM_Event_Queue_Address *origin, SVM_Structure *event, const unsigned long int timeout, const SVM_Boolean soft);

/** \} */

/** \} */

/** \defgroup processors SVM processors
 * This module contains API functions to handle SVM processors.
 *
 * A processor is a main actor in the virtual machine code execution.
 * 
 * It controls instruction execution flow written in a SVM code within a kernel.
 *
 * \see kernels
 * \see codes
 * \{
 */

/** \defgroup executionprocessor SVM processor execution
 * This module contains API functions to handle SVM processor execution.
 * \{
 */

/**
 * This function extracts the SVM code actually executed by the processor.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel from which the code  will be extracted.
 *
 * \returns The SVM code executed by the processor.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 *
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 */
SVM_FUNCTION SVM_Code svm_processor_get_currentcode(const void *svm, const SVM_Kernel kernel);

/**
 * This function retrieves the address of the next instruction a processor will execute.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted processor is.
 *
 * \returns The symbol pointing on the next instruction.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 *
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 */
SVM_FUNCTION SVM_Value_Symbol svm_processor_get_nextinstruction(const void *svm, const SVM_Kernel kernel);

/**
 * This function retrieves the address of the current instruction a processor is executing.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted processor is.
 *
 * \returns The symbol pointing on the current instruction.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 *
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 * \see svm_value_symbol_get_location
 */
SVM_FUNCTION SVM_Value_Symbol svm_processor_get_currentinstruction(const void *svm, const SVM_Kernel kernel);

/**
 * This function does a local jump on a processor.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted processor is.
 * \param[in] address The address to jump to.
 *
 * The next instruction pointer of the processor is set to an address inside the current SVM code.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 *
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 */
SVM_FUNCTION void svm_processor_jump_local(const void *svm, const SVM_Kernel kernel, const SVM_Address address);

/**
 * This function does a global jump on a processor.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted processor is.
 * \param[in] symbol The address to jump to.
 *
 * The next instruction pointer of the processor is set to an address potentially outside the current SVM code.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 *
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 */
SVM_FUNCTION void svm_processor_jump_global(const void *svm, const SVM_Kernel kernel, const SVM_Value_Symbol symbol);

/**
 * This function does a local SVM code function call on a processor.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted processor is.
 * \param[in] function The address of the function to jump to.
 * \param[in] parameters The memory pointer locating the function parameters.
 *
 * The current state of the processor is saved on the return stack, then the next instruction is set to the function address inside the current SVM code executed by the processor, and the current memory pointer is set to the parameters location.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 *
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 */
SVM_FUNCTION void svm_processor_call_local(const void *svm, const SVM_Kernel kernel, const SVM_Address function, const SVM_Value_Pointer parameters);

/**
 * This function does a global SVM code function call on a processor.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted processor is.
 * \param[in] function The address of the function to jump to.
 * \param[in] parameters The memory pointer locating the function parameters.
 *
 * The current state of the processor is saved on the return stack, then the next instruction is set to the function address eventually outside the current SVM code executed by the processor, and the current memory pointer is set to the parameters location.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 *
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 */
SVM_FUNCTION void svm_processor_call_global(const void *svm, const SVM_Kernel kernel, const SVM_Value_Symbol function, const SVM_Value_Pointer parameters);

/**
 * This function does an identical SVM code function call on a processor.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted processor is.
 * \param[in] transfer Indicates whether the local memory shall be transferred to the call.
 *
 * The current state of the processor is saved on the return stack.
 * The memory already allocated in the current function can not be managed by the two calls:
 * The memory can be managed by the current function call or transferred to the new function call.
 * If the return stack is not modified, it is advised to set the transfer boolean to TRUE.
 *
 * \note This API function can be used to store the current state of the processor, but has to be used with caution.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 *
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 */
SVM_FUNCTION void svm_processor_call_identical(const void *svm, const SVM_Kernel kernel, const SVM_Boolean transfer);

/**
 * This function does a SVM code function return on a processor.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted processor is.
 *
 * The current state is replaced by the last pushed state on the return stack, the last pushed state is removed from the return stack, and the allocated memory associated to the destroyed state is deleted.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 * \throws PROCESSOR interruption when the return stack is empty.
 *
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 */
SVM_FUNCTION void svm_processor_return(const void *svm, const SVM_Kernel kernel);

/**
 * This function shuts down the processor of the current kernel.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION void svm_processor_current_shutdown(const void *svm);

/**
 * This function stops the execution of the processor of the current kernel for a given time as integer values.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] seconds The number of seconds to wait.
 * \param[in] milliseconds The number of extra milliseconds to wait.
 * \param[in] soft The process is put in pause while waiting.
 *
 * This function put the process on hold for a given while.
 *
 * \note In protected mode, this function enforces a process pause and resume around the sleep time, if not done using API functions.
 *
 * \warning Usage of the system sleep functions may result in an undefined behavior. Always use this function instead.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when a sleep is performed in protected mode outside a WAITING instruction.
 * \throws FAILURE interruption when this API function is called outside a kernel.
 * \throws FAILURE interruption when seconds or milliseconds are a negative integer.
 *
 * \see svm_process_pause
 * \see svm_process_resume
 * \see svm_kernel_get_current
 */
SVM_FUNCTION void svm_processor_current_sleep(const void *svm, const SVM_Value_Integer seconds, const SVM_Value_Integer milliseconds, const SVM_Boolean soft);

/**
 * This function stops the execution of the processor of the current kernel for a given time as raw integers.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] seconds The number of seconds to wait.
 * \param[in] milliseconds The number of extra milliseconds to wait.
 * \param[in] soft The process is put in pause while waiting.
 *
 * This function put the process on hold for a given while.
 *
 * \note In protected mode, this function enforces a process pause and resume around the sleep time, if not done using API functions.
 *
 * \warning Usage of the system sleep functions may result in an undefined behavior. Always use this function instead.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when this API function is called outside a kernel.
 * \throws FAILURE interruption when a sleep is performed in protected mode outside a WAITING instruction.
 *
 * \see svm_process_pause
 * \see svm_process_resume
 * \see svm_kernel_get_current
 */
SVM_FUNCTION void svm_processor_current_sleep__raw(const void *svm, const unsigned long int seconds, const unsigned long int milliseconds, const SVM_Boolean soft);

/** \} */

/** \defgroup returnstackprocessors SVM processor return stack
 * This module contains API functions to handle SVM processor function return stack.
 * \{
 */

/**
 * This function retrieves the return stack size of a processor.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted processor is.
 *
 * \returns The number of function returns the processor can do.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 *
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 */
SVM_FUNCTION SVM_Size svm_processor_returnstack_get_size(const void *svm, const SVM_Kernel kernel);

/**
 * This function swaps a state in the return stack of a processor with the current state of this processor.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted processor is.
 * \param[in] level The level of the state to be swaped in the return stack.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 * \throws PROCESSOR interruption when the return stack does not contain enough levels.
 *
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 */
SVM_FUNCTION void svm_processor_returnstack_swap_level(const void *svm, const SVM_Kernel kernel, const SVM_Index level);

/**
 * This function returns the level of the nearest frame having the flag raised within the return stack of a processor from a given level.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted processor is.
 * \param[in] flag The flag name to find.
 * \param[in] start The level of the state to be swaped in the return stack.
 *
 * The start has to be set between 0 and the return stack size. When the return stack size is provided, no search is done and the return stack size is immediately returned.
 *
 * \return Returns the first level when the flag is found. Returns the return stack size when not found.
 * 
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 * \throws PROCESSOR interruption when the return stack does not contain enough levels.
 *
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 */
SVM_FUNCTION SVM_Index svm_processor_returnstack_find_flag(const void *svm, const SVM_Kernel kernel, const SVM_Value_String flag, const SVM_Index start);

/**
 * This function returns the level of the nearest frame having the flag raised within the return stack of a processor from a given level.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted processor is.
 * \param[in] flag The flag name to find.
 * \param[in] start The level of the state to be swaped in the return stack.
 *
 * The start has to be set between 0 and the return stack size. When the return stack size is provided, no search is done and the return stack size is immediately returned.
 *
 * \return Returns the first level when the flag is found. Returns the return stack size when not found.
 * 
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 * \throws PROCESSOR interruption when the return stack does not contain enough levels.
 *
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 */
SVM_FUNCTION SVM_Index svm_processor_returnstack_find_flag__string(const void *svm, const SVM_Kernel kernel, const SVM_String flag, const SVM_Index start);

/**
 * This function returns the level of the nearest frame having the flag raised within the return stack of a processor from a given level.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted processor is.
 * \param[in] flag The flag name to find.
 * \param[in] start The level of the state to be swaped in the return stack.
 *
 * The start has to be set between 0 and the return stack size. When the return stack size is provided, no search is done and the return stack size is immediately returned.
 *
 * \return Returns the first level when the flag is found. Returns the return stack size when not found.
 * 
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 * \throws PROCESSOR interruption when the return stack does not contain enough levels.
 *
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 */
SVM_FUNCTION SVM_Index svm_processor_returnstack_find_flag__raw(const void *svm, const SVM_Kernel kernel, const char *flag, const SVM_Index start);

/**
 * This function moves a portion of the return stack to another level.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted processor is.
 * \param[in] start The first return stack level to move.
 * \param[in] end The first return stack level after the portion to move.
 * \param[in] target The level where to move the portion of the return stack.
 *
 * After the move, the first level of the moved is placed at the target.
 *
 * \note The start and end levels can be between 0 and the return stack size included, and the start shall be inferior to the end.
 * \note The target can be between 0 and the size of the stack minus the portion size, included.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 * \throws FAILURE interruption when the start and end levels are inconsistent.
 * \throws PROCESSOR interruption when the return stack does not contain enough levels.
 * \throws PROCESSOR interruption when the move targets a level outside the return stack.
 *
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 */
SVM_FUNCTION void svm_processor_returnstack_move_level(const void *svm, const SVM_Kernel kernel, const SVM_Index start, const SVM_Index end, const SVM_Index target);

/** \} */

/** \defgroup memoryprocessors SVM processor memory
 * This module contains API functions to handle SVM processor memory.
 * \{
 */

/**
 * This function extracts the current memory pointer of a processor.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted processor is.
 *
 * \returns The current pointer value.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 *
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 */
SVM_FUNCTION SVM_Value_Pointer svm_processor_get_currentpointer(const void *svm, const SVM_Kernel kernel);

/**
 * This function modifies the current memory pointer of a processor.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted processor is.
 * \param[in] pointer The new value of the current memory pointer.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 *
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 */
SVM_FUNCTION void svm_processor_set_currentpointer(const void *svm, const SVM_Kernel kernel, const SVM_Value_Pointer pointer);

/** \} */

/** \defgroup interruptionprocessors SVM processor interruption
 * This module contains API functions to handle SVM processor interruption.
 * \{
 */

/**
 * This function sets a local interruption handler on a processor.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted processor is.
 * \param[in] interruption The interruption for which the handler has to be set.
 * \param[in] address The local address of the handler.
 * \param[in] level The level of interruption.
 *
 * \note The interruption message and kind are ignored.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 *
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 */
SVM_FUNCTION void svm_processor_interruptionhandler_set_local(const void *svm, const SVM_Kernel kernel, const SVM_Value_Interruption interruption, const SVM_Address address, const SVM_Object_Level level);

/**
 * This function sets a global interruption handler on a processor.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted processor is.
 * \param[in] interruption The interruption for which the handler has to be set.
 * \param[in] symbol The global address of the handler.
 * \param[in] level The level of interruption.
 *
 * \note The interruption message and kind are ignored.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 *
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 */
SVM_FUNCTION void svm_processor_interruptionhandler_set_global(const void *svm, const SVM_Kernel kernel, const SVM_Value_Interruption interruption, const SVM_Value_Symbol symbol, const SVM_Object_Level level);

/**
 * This function removes an interruption handler from a processor.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted processor is.
 * \param[in] interruption The interruption for which the handler has to be reset.
 * \param[in] level The level of interruption.
 *
 * \note The interruption message and kind are ignored.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 *
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 */
SVM_FUNCTION void svm_processor_interruptionhandler_reset(const void *svm, const SVM_Kernel kernel, const SVM_Value_Interruption interruption, const SVM_Object_Level level);

/**
 * This function checks whether an interruption handler exists in a processor.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted processor is.
 * \param[in] interruption The interruption for which the handler has to be extracted.
 * \param[in] level The level of interruption.
 *
 * \note The interruption message and kind are ignored.
 *
 * \note The level can be LOCAL or GLOBAL. Cascaded interruptions are considered local by this API function.
 *
 * \returns TRUE if an handler exists in the current state of the procerror, FALSE otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 *
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 */
SVM_FUNCTION SVM_Boolean svm_processor_interruptionhandler_has(const void *svm, const SVM_Kernel kernel, const SVM_Value_Interruption interruption, const SVM_Object_Level level);

/**
 * This function extracts an interruption handler from a processor.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted processor is.
 * \param[in] interruption The interruption for which the handler has to be extracted.
 * \param[in] level The level of interruption.
 *
 * \note The interruption message and kind are ignored.
 *
 * \note The level can be LOCAL or GLOBAL. Cascaded interruptions are considered local by this API function.
 *
 * \returns The symbol associated to the interruption handler.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 * \throws PROCESSOR interruption when the interruption handler does not exist.
 *
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 */
SVM_FUNCTION SVM_Value_Symbol svm_processor_interruptionhandler_get(const void *svm, const SVM_Kernel kernel, const SVM_Value_Interruption interruption, const SVM_Object_Level level);

/**
 * This function interrupts a processor.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted processor is.
 * \param[in] interruption The interruption to append in the interruption list of the processor.
 *
 * \note When applied on the current processor, the execution of the current instruction is not stopped.
 * The interruption will occur after the instruction execution completion.
 * To interrupt also the current instruction execution, please refer to the svm_processor_current_raise_error API function.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 *
 * \see svm_processor_current_raise_error
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 */
SVM_FUNCTION void svm_processor_add_interruption(const void *svm, const SVM_Kernel kernel, const SVM_Value_Interruption interruption);

/**
 * This function clears waiting interruptions of a processor.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted processor is.
 *
 * This function removes all waiting interruptions of a processor.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 *
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 */
SVM_FUNCTION void svm_processor_clear_interruption(const void *svm, const SVM_Kernel kernel);

/**
 * This function retrieves waiting interruptions of a processor.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted processor is.
 *
 * \returns A pointer to a C array of SVM_Value_Interruption, terminated by a C/C++ null pointer.
 *
 * \note The returned array is a valid SVM variable: its scope can be changed if needed, and the pointer shall not be freed explicitely.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 *
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 */
SVM_FUNCTION SVM_Value_Interruption* svm_processor_list_interruption(const void *svm, const SVM_Kernel kernel);

/**
 * This function disables waiting interruptions handling of a processor.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted processor is.
 *
 * This function disables waiting interruption handling by the processor.
 * Waiting interruptions will be put on hold by the processor and will accumulate all raised interruptions.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 *
 * \see svm_processor_release_interruption
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 */
SVM_FUNCTION void svm_processor_hold_interruption(const void *svm, const SVM_Kernel kernel);

/**
 * This function enables waiting interruptions handling of a processor.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted processor is.
 *
 * This function enables waiting interruption handling by the processor.
 * Waiting interruptions will be taken into account as soon as possible by the processor.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 *
 * \see svm_processor_hold_interruption
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 */
SVM_FUNCTION void svm_processor_release_interruption(const void *svm, const SVM_Kernel kernel);

/**
 * This function detects whether an hardware interruption has been received by a processor.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted processor is.
 *
 * This API function can be useful to interrupt blocking instructions on a signal.
 *
 * \returns TRUE if the processor received at least one hardware interruption, FALSE otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 *
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 */
SVM_FUNCTION SVM_Boolean svm_processor_is_hardwareinterrupted(const void *svm, const SVM_Kernel kernel);

/** \} */

/** \defgroup errorprocessors SVM processor error
 * This module contains API functions to handle SVM processor error.
 * \{
 */

/**
 * This function interrupts the current processor and the current instruction execution from any interruption.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] interruption The interruption to raise to the processor.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION void svm_processor_current_raise_error(const void *svm, const SVM_Value_Interruption interruption);

/**
 * This function interrupts the current processor and the current instruction execution from an internal interruption.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] interruption The interruption type to raise to the processor.
 * \param[in] message The message for investigation of the root cause of the interruption.
 *
 * The interruption kind is automatically set to SOFTWARE.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the interruption type is PLUGIN_INTERRUPTION.
 */
SVM_FUNCTION void svm_processor_current_raise_error_internal__raw(const void *svm, const SVM_Interruption_Internal interruption, const char *message);

/**
 * This function interrupts the current processor and the current instruction execution from a plugin defined interruption.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] interruption The interruption name to raise to the processor.
 * \param[in] message The message for investigation of the root cause of the interruption.
 *
 * The interruption type is automatically set to PLUGIN_INTERRUPTION.
 *
 * The interruption kind is automatically set to SOFTWARE.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the interruption parameter does not identify a plugin interruption.
 */
SVM_FUNCTION void svm_processor_current_raise_error_external__raw(const void *svm, const SVM_Value_PluginEntryPoint interruption, const char *message);

/** \} */

/** \defgroup flagprocessors SVM processor flags
 * This module contains API functions to handle SVM processor flags.
 * \{
 */

/**
 * This function detects whether a flag is raised in a processor current state, from a string value.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted processor is.
 * \param[in] flag The flag name to check.
 *
 * \returns TRUE if the flag is raised in the current processor state, FALSE otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 *
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 */
SVM_FUNCTION SVM_Boolean svm_processor_has_flag(const void *svm, const SVM_Kernel kernel, const SVM_Value_String flag);

/**
 * This function detects whether a flag is raised in a processor current state, from a SVM_String.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted processor is.
 * \param[in] flag The flag name to check.
 *
 * \returns TRUE if the flag is raised in the current processor state, FALSE otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 *
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 */
SVM_FUNCTION SVM_Boolean svm_processor_has_flag__string(const void *svm, const SVM_Kernel kernel, const SVM_String flag);

/**
 * This function detects whether a flag is raised in a processor current state, from a C nul-terminated string.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted processor is.
 * \param[in] flag The flag name to check.
 *
 * \returns TRUE if the flag is raised in the current processor state, FALSE otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 *
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 */
SVM_FUNCTION SVM_Boolean svm_processor_has_flag__raw(const void *svm, const SVM_Kernel kernel, const char *flag);

/**
 * This function lists all raised flags in a processor current state.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted processor is.
 *
 * \returns A NULL terminated array of string values containing the flag names.
 *
 * \note The returned array is a valid SVM variable: its scope can be changed if needed, and the pointer shall not be freed explicitely.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 *
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 */
SVM_FUNCTION SVM_Value_String* svm_processor_list_flag(const void *svm, const SVM_Kernel kernel);

/**
 * This function raises a flag in a processor current state, from a string value.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted processor is.
 * \param[in] flag The flag name to raise.
 * \param[in] level The level of flag.
 *
 * This API function does nothing if the flag was already raised.
 *
 * \note The level can be only LOCAL or CASCADE.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 *
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 */
SVM_FUNCTION void svm_processor_set_flag(const void *svm, const SVM_Kernel kernel, const SVM_Value_String flag, const SVM_Object_Level level);

/**
 * This function raises a flag in a processor current state, from a SVM_String.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted processor is.
 * \param[in] flag The flag name to raise.
 * \param[in] level The level of flag.
 *
 * This API function does nothing if the flag was already raised.
 *
 * \note The level can be only LOCAL or CASCADE.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 *
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 */
SVM_FUNCTION void svm_processor_set_flag__string(const void *svm, const SVM_Kernel kernel, const SVM_String flag, const SVM_Object_Level level);

/**
 * This function raises a flag in a processor current state, from a C nul-terminated string.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted processor is.
 * \param[in] flag The flag name to raise.
 * \param[in] level The level of flag.
 *
 * This API function does nothing if the flag was already raised.
 *
 * \note The level can be only LOCAL or CASCADE.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 *
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 */
SVM_FUNCTION void svm_processor_set_flag__raw(const void *svm, const SVM_Kernel kernel, const char *flag, const SVM_Object_Level level);

/**
 * This function lowers a flag in a processor current state, from a string value.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted processor is.
 * \param[in] flag The flag name to lower.
 * \param[in] level The level of flag.
 *
 * This API function does nothing if the flag was already lowered.
 *
 * \note The level can be only LOCAL or CASCADE.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 *
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 */
SVM_FUNCTION void svm_processor_reset_flag(const void *svm, const SVM_Kernel kernel, const SVM_Value_String flag, const SVM_Object_Level level);

/**
 * This function lowers a flag in a processor current state, from a SVM_String.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted processor is.
 * \param[in] flag The flag name to lower.
 * \param[in] level The level of flag.
 *
 * This API function does nothing if the flag was already lowered.
 *
 * \note The level can be only LOCAL or CASCADE.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 *
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 */
SVM_FUNCTION void svm_processor_reset_flag__string(const void *svm, const SVM_Kernel kernel, const SVM_String flag, const SVM_Object_Level level);

/**
 * This function lowers a flag in a processor current state, from a C nul-terminated string.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted processor is.
 * \param[in] flag The flag name to lower.
 * \param[in] level The level of flag.
 *
 * This API function does nothing if the flag was already lowered.
 *
 * \note The level can be only LOCAL or CASCADE.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 *
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 */
SVM_FUNCTION void svm_processor_reset_flag__raw(const void *svm, const SVM_Kernel kernel, const char *flag, const SVM_Object_Level level);

/** \} */

/** \defgroup instructionoverrideprocessors SVM processor instruction overrides
 * This module contains API functions to handle SVM processor instruction overrides.
 * \{
 */

/**
 * This function sets an instruction override on a local address.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted processor is.
 * \param[in] address The local address to override.
 * \param[in] name The instruction name.
 * \param[in] argc The number of instruction parameters.
 * \param[in] argv The instruction parameters.
 * \param[in] level The level of instruction override.
 *
 * This API function sets an instruction override on the processor, at the given address of the current code of the processor.
 * This instruction will be executed when the current instruction pointer matches the address, replacing the code one at the same address location.
 *
 * \note The created instruction will have the code location of the instruction calling this function. Parameter checks will be performed when the instruction is executed.
 *
 * \warning Only plugin instructions are allowed. Return values from these instructions are lost.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 * \throws FAILURE interruption when the address already has an instruction override.
 * \throws PROCESSOR interruption when the instruction does not exist.
 *
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 * \see svm_plugin_has_instruction
 */
SVM_FUNCTION void svm_processor_instructionoverride_set_local(const void *svm, const SVM_Kernel kernel, const SVM_Address address, const SVM_Value_PluginEntryPoint name, const SVM_Size argc, const SVM_Parameter argv[], const SVM_Object_Level level);

/**
 * This function sets an instruction override on a local address.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted processor is.
 * \param[in] symbol The symbol to override.
 * \param[in] name The instruction name.
 * \param[in] argc The number of instruction parameters.
 * \param[in] argv The instruction parameters.
 * \param[in] level The level of instruction override.
 *
 * This API function sets an instruction override on the processor, at the given symbol.
 * This instruction will be executed when the current instruction pointer matches the symbol, replacing the code one at the same symbol location.
 *
 * \note The created instruction will have the code location of the instruction calling this function. Parameter checks will be performed when the instruction is executed.
 *
 * \warning Only plugin instructions are allowed. Return values from these instructions are lost.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 * \throws FAILURE interruption when the symbol already has an instruction override.
 * \throws PROCESSOR interruption when the instruction does not exist.
 *
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 * \see svm_plugin_has_instruction
 */
SVM_FUNCTION void svm_processor_instructionoverride_set_global(const void *svm, const SVM_Kernel kernel, const SVM_Value_Symbol symbol, const SVM_Value_PluginEntryPoint name, const SVM_Size argc, const SVM_Parameter argv[], const SVM_Object_Level level);

/**
 * This function checks whether a processor has an instruction override on a local address.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted processor is.
 * \param[in] address The local address to check.
 * \param[in] level The level of instruction override.
 *
 * \returns TRUE when the address has an instruction override, FALSE otherwise.
 *
 * \note The level can be LOCAL or GLOBAL. Cascaded instruction overrides are considered local by this API function.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 *
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 */
SVM_FUNCTION SVM_Boolean svm_processor_instructionoverride_has_local(const void *svm, const SVM_Kernel kernel, const SVM_Address address, const SVM_Object_Level level);

/**
 * This function checks whether a processor has an instruction override on a symbol.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted processor is.
 * \param[in] symbol The symbol to check.
 * \param[in] level The level of instruction override.
 *
 * \returns TRUE when the symbol has an instruction override, FALSE otherwise.
 *
 * \note The level can be LOCAL or GLOBAL. Cascaded instruction overrides are considered local by this API function.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 *
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 */
SVM_FUNCTION SVM_Boolean svm_processor_instructionoverride_has_global(const void *svm, const SVM_Kernel kernel, const SVM_Value_Symbol symbol, const SVM_Object_Level level);

/**
 * This function removes an instruction override on a local address.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted processor is.
 * \param[in] address The local address where the override shall be removed.
 * \param[in] level The level of instruction override.
 *
 * This function removes an instruction override at the local address of the current code.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 *
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 */
SVM_FUNCTION void svm_processor_instructionoverride_reset_local(const void *svm, const SVM_Kernel kernel, const SVM_Address address, const SVM_Object_Level level);

/**
 * This function removes an instruction override on a symbol.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted processor is.
 * \param[in] symbol The symbol where the override shall be removed.
 * \param[in] level The level of instruction override.
 *
 * This function removes an instruction override at the symbol.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 *
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 */
SVM_FUNCTION void svm_processor_instructionoverride_reset_global(const void *svm, const SVM_Kernel kernel, const SVM_Value_Symbol symbol, const SVM_Object_Level level);

/** \} */

/** \} */

/** \defgroup memories SVM memories
 * This module contains API functions to handle SVM memories.
 *
 * A memory is a data storage using strong typing and allocation to control data integrity and life span.
 *
 * \see kernels
 * \see values
 * \{
 */

/** \defgroup memoryzone SVM memory zone
 * This module contains API functions to handle SVM memory zones.
 * \{
 */

/**
 * This function creates an empty memory zone description.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 *
 * \see svm_memory_pointer_get_zone
 */
SVM_FUNCTION SVM_Memory_Zone svm_memory_zone_new(const void *svm);

/**
 * This function appends an array of types of length given as integer value.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in,out] zone The memory zone to extend.
 * \param[in] type The type to append.
 * \param[in] times The size of the array.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the times parameter is a negative integer.
 */
SVM_FUNCTION void svm_memory_zone_append(const void *svm, SVM_Memory_Zone zone, const SVM_Type type, const SVM_Value_Integer times);

/**
 * This function appends an array of types of length given as integer value.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in,out] zone The memory zone to extend.
 * \param[in] type The type to append.
 * \param[in] times The size of the array.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the times parameter is a negative integer.
 */
SVM_FUNCTION void svm_memory_zone_append__raw(const void *svm, SVM_Memory_Zone zone, const SVM_Type type, const SVM_Size times);

/**
 * This function appends an array of internal types of length given as integer value.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in,out] zone The memory zone to extend.
 * \param[in] type The internal type to append.
 * \param[in] times The size of the array.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the times parameter is a negative integer.
 */
SVM_FUNCTION void svm_memory_zone_append_internal(const void *svm, SVM_Memory_Zone zone, const SVM_Type_Internal type, const SVM_Value_Integer times);

/**
 * This function appends an array of internal types of length given as raw integer.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in,out] zone The memory zone to extend.
 * \param[in] type The internal type to append.
 * \param[in] times The size of the array.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION void svm_memory_zone_append_internal__raw(const void *svm, SVM_Memory_Zone zone, const SVM_Type_Internal type, const SVM_Size times);

/**
 * This function appends an array of plugin types of length given as integer value.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in,out] zone The memory zone to extend.
 * \param[in] type The plugin type name to append.
 * \param[in] times The size of the array.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the times parameter is a negative integer.
 */
SVM_FUNCTION void svm_memory_zone_append_external(const void *svm, SVM_Memory_Zone zone, const SVM_Value_PluginEntryPoint type, const SVM_Value_Integer times);

/**
 * This function appends an array of plugin types of length given as raw integer.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in,out] zone The memory zone to extend.
 * \param[in] type The plugin type name to append.
 * \param[in] times The size of the array.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION void svm_memory_zone_append_external__raw(const void *svm, SVM_Memory_Zone zone, const SVM_Value_PluginEntryPoint type, const SVM_Size times);

/**
 * This function appends a memory zone to a memory zone.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in,out] zone The memory zone to extend.
 * \param[in] append The memory zone to append.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION void svm_memory_zone_append_memory_zone(const void *svm, SVM_Memory_Zone zone, const SVM_Memory_Zone append);

/**
 * This function inserts a memory zone into a memory zone.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in,out] zone The memory zone to extend.
 * \param[in] index The index where the memory zone is inserted.
 * \param[in] insert The memory zone to insert.
 *
 * The index can be set between 0 (begin) to the memory zone size (end).
 *
 * \note When the index is equal to the memory zone size, this instruction is equivalent to svm_memory_zone_append_memory_zone.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE when the index is outside the memory zone.
 */
SVM_FUNCTION void svm_memory_zone_insert_memory_zone(const void *svm, SVM_Memory_Zone zone, const SVM_Index index, const SVM_Memory_Zone insert);

/**
 * This function remove a part within a memory zone.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in,out] zone The memory zone to extend.
 * \param[in] index The index where the memory zone is deleted.
 * \param[in] size The size of the removed part of the memory zone.
 *
 * The index can be set between 0 (begin) to the memory zone size (end).
 *
 * \note When the index is equal to the memory zone size, this instruction does nothing.
 *
 * \throws FA)FA
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE when the index is outside the memory zone.
 * \throws FAILURE when the index+size is outside the memory zone.
 */
SVM_FUNCTION void svm_memory_zone_remove_memory_zone(const void *svm, SVM_Memory_Zone zone, const SVM_Index index, const SVM_Size size);

/**
 * This function replace a memory zone inside a memory zone.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in,out] zone The memory zone to change.
 * \param[in] index The index where the memory zone is replaced.
 * \param[in] replace The memory zone to replace.
 *
 * The index can be set between 0 (begin) to the memory zone size (end).
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE when the index is outside the memory zone.
 * \throws FAILURE when the replacement memory zone is outside the memory zone.
 */
SVM_FUNCTION void svm_memory_zone_replace_memory_zone(const void *svm, SVM_Memory_Zone zone, const SVM_Index index, const SVM_Memory_Zone replace);

/**
 * This function finds a memory zone inside a memory zone.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in,out] zone The memory zone to search in.
 * \param[in] index The index where the search starts.
 * \param[in] find The memory zone pattern to search.
 *
 * \returns The index of the first occurence when the pattern memory zone is found, and the memory zone size when not found.
 *
 * The index can be set between 0 (begin) to the memory zone size (end).
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE when the index is outside the memory zone.
 */
SVM_FUNCTION SVM_Index svm_memory_zone_find_memory_zone(const void *svm, SVM_Memory_Zone zone, const SVM_Index index, const SVM_Memory_Zone find);

/**
 * This function returns the memory zone size.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in,out] zone The memory zone to check.
 *
 * \returns The number of types inside the memory zone.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Size svm_memory_zone_get_size(const void *svm, const SVM_Memory_Zone zone);

/**
 * This function returns one memory zone element.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in,out] zone The memory zone to read.
 * \param[in] index The index to read.
 *
 * The index can be set between 0 (begin) to the memory zone size-1 (end).
 *
 * \returns The type at the given index within the memory zone.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Type svm_memory_zone_get_type(const void *svm, const SVM_Memory_Zone zone, const SVM_Index index);

/**
 * This function creates a copy of a memory zone.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] zone The memory zone to copy.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Memory_Zone svm_memory_zone_copy(const void *svm, const SVM_Memory_Zone zone);

/**
 * This function creates a string representation of a memory zone.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] zone The memory zone to print.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_String svm_memory_zone_print(const void *svm, const SVM_Memory_Zone zone);

/** \} */

/** \defgroup allocationmemory SVM memory allocation
 * This module contains API functions to handle SVM memory allocation.
 * \{
 */

/**
 * This function allocates a zone in a memory.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted memory is.
 * \param[in] zone The zone description to allocate.
 *
 * \warning The allocated memory is local and will be freed at the end of the current SVM function.
 * Please refer to the svm_memory_scope_set_global and svm_memory_pointer_list_accessible API functions to create global memory.
 *
 * \note The allocated memory is not initialised. It means each allocated addresses contains a type but no value.
 *
 * \returns The pointer containing the zone location.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 *
 * \see svm_memory_scope_set_global
 * \see svm_memory_pointer_list_accessible
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 */
SVM_FUNCTION SVM_Value_Pointer svm_memory_allocate(const void *svm, const SVM_Kernel kernel, const SVM_Memory_Zone zone);

/**
 * This function allocates a zone in a memory at a specified address.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted memory is.
 * \param[in] zone The zone description to allocate.
 * \param[in] address The address where to allocate the memory.
 *
 * \warning The allocated memory is local and will be freed at the end of the current SVM function.
 * Please refer to the svm_memory_scope_set_global and svm_memory_pointer_list_accessible API functions to create global memory.
 *
 * \note The allocated memory is not initialised. It means each allocated addresses contains a type but no value.
 *
 * \returns The pointer containing the zone location when the allocation succeeds. A null pointer is returned otherwise.
 *
 * Example:
 * \code{c}
 * SVM_Value_Pointer p = svm_memory_allocate_address(svm,svm_kernel_get_current(svm),zone,address);
 * if(svm_value_state_is_null(svm,p))
 * {
 *     svm_processor_current_raise_error_internal__raw(svm,MEMORY,"Unable to reserve memory");
 * }
 * \endcode
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 *
 * \see svm_memory_scope_set_global
 * \see svm_memory_pointer_list_accessible
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 * \see svm_value_state_is_null
 */
SVM_FUNCTION SVM_Value_Pointer svm_memory_allocate_address(const void *svm, const SVM_Kernel kernel, const SVM_Memory_Zone zone, const SVM_Address address);

/**
 * This function frees a memory zone.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted memory is.
 * \param[in] pointer The pointer locating the memory zone.
 *
 * The targeted addresses are removed from the memory.
 *
 * \note The scope of the memory is unchanged.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 *
 * \see svm_memory_scope_set_local
 */
SVM_FUNCTION void svm_memory_free(const void *svm, const SVM_Kernel kernel, const SVM_Value_Pointer pointer);

/** \} */

/** \defgroup memoriesscope SVM memory scope
 * This module contains API functions to handle SVM memory scope.
 *
 * A memory scope corresponds to the life time of memory addresses.
 *
 * \see kernels
 * \{
 */

/**
 * This function changes the scope of a memory zone.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted memory is.
 * \param[in] pointer The pointer locating the memory zone.
 *
 * The targeted addresses are put in the processor current state and will be freed at the end of the SVM function.
 *
 * This API function allows delegation of memory liberation at the end of the current function.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 *
 * \see svm_memory_scope_set_local_alias
 * \see svm_memory_scope_set_local_alias__string
 * \see svm_memory_scope_set_local_alias__raw
 */
SVM_FUNCTION void svm_memory_scope_set_local(const void *svm, const SVM_Kernel kernel, const SVM_Value_Pointer pointer);

/**
 * This function changes the scope of a memory zone.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted memory is.
 * \param[in] pointer The pointer locating the memory zone.
 *
 * The targeted addresses are removed from the processor current state and will not be freed at the end of the SVM function.
 *
 * This API function allows delegation of memory liberation at the end of the current function.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 *
 * \see svm_memory_scope_set_global_alias
 * \see svm_memory_scope_set_global_alias__string
 * \see svm_memory_scope_set_global_alias__raw
 */
SVM_FUNCTION void svm_memory_scope_set_global(const void *svm, const SVM_Kernel kernel, const SVM_Value_Pointer pointer);

/**
 * This function changes the scope of a memory alias from a string value.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted memory is.
 * \param[in] alias The alias to change.
 *
 * The targeted alias is put in the processor current state and will be freed at the end of the SVM function.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 *
 * \see svm_memory_scope_set_local
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 */
SVM_FUNCTION void svm_memory_scope_set_local_alias(const void *svm, const SVM_Kernel kernel, const SVM_Value_String alias);

/**
 * This function changes the scope of a memory alias from a SVM_String.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted memory is.
 * \param[in] alias The alias to change.
 *
 * The targeted alias is put in the processor current state and will be freed at the end of the SVM function.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 *
 * \see svm_memory_scope_set_local
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 */
SVM_FUNCTION void svm_memory_scope_set_local_alias__string(const void *svm, const SVM_Kernel kernel, const SVM_String alias);

/**
 * This function changes the scope of a memory alias from a C nul-terminated string.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted memory is.
 * \param[in] alias The alias to change.
 *
 * The targeted alias is put in the processor current state and will be freed at the end of the SVM function.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 *
 * \see svm_memory_scope_set_local
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 */
SVM_FUNCTION void svm_memory_scope_set_local_alias__raw(const void *svm, const SVM_Kernel kernel, const char *alias);

/**
 * This function changes the scope of a memory alias from a string value.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted memory is.
 * \param[in] alias The alias to change.
 *
 * The targeted alias is removed from the processor current state and will not be freed at the end of the SVM function.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 *
 * \see svm_memory_scope_set_local
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 */
SVM_FUNCTION void svm_memory_scope_set_global_alias(const void *svm, const SVM_Kernel kernel, const SVM_Value_String alias);

/**
 * This function changes the scope of a memory alias from a SVM_String.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted memory is.
 * \param[in] alias The alias to change.
 *
 * The targeted alias is removed from the processor current state and will not be freed at the end of the SVM function.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 *
 * \see svm_memory_scope_set_local
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 */
SVM_FUNCTION void svm_memory_scope_set_global_alias__string(const void *svm, const SVM_Kernel kernel, const SVM_String alias);

/**
 * This function changes the scope of a memory alias from a C nul-terminated string.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted memory is.
 * \param[in] alias The alias to change.
 *
 * The targeted alias is removed from the processor current state and will not be freed at the end of the SVM function.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 *
 * \see svm_memory_scope_set_local
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 */
SVM_FUNCTION void svm_memory_scope_set_global_alias__raw(const void *svm, const SVM_Kernel kernel, const char *alias);

/** \} */

/** \defgroup addressmemory SVM memory addresses
 * This module contains API functions to handle SVM memory addresses.
 * \{
 */

/**
 * This function checks whether an address is defined in a memory.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted memory is.
 * \param[in] address The local address to check.
 *
 * \returns TRUE if the address has been allocated, FALSE otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 *
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 */
SVM_FUNCTION SVM_Boolean svm_memory_address_is_defined(const void *svm, const SVM_Kernel kernel, const SVM_Address address);

/**
 * This function checks whether an address is initialised in a memory.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted memory is.
 * \param[in] address The local address to check.
 *
 * \returns TRUE if the address has been allocated and has been initialised, FALSE otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 *
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 */
SVM_FUNCTION SVM_Boolean svm_memory_address_is_initialised(const void *svm, const SVM_Kernel kernel, const SVM_Address address);

/**
 * This function extracts the type of a value from a memory.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted memory is.
 * \param[in] address The local address to check.
 *
 * \returns The type of the value.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 * \throws MEMORY interruption when the address is not defined.
 *
 * \see svm_memory_address_is_defined
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 */
SVM_FUNCTION SVM_Type svm_memory_address_get_type(const void *svm, const SVM_Kernel kernel, const SVM_Address address);

/**
 * This function checks whether a value can be written into a memory.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted memory is.
 * \param[in] address The local address to write to.
 * \param[in] value The value to check.
 *
 * This API function does not write the value into the memory.
 *
 * \returns TRUE when the write can be successful, FALSE otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 *
 * \see svm_memory_write_address
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 */
SVM_FUNCTION SVM_Boolean svm_memory_address_is_writable(const void *svm, const SVM_Kernel kernel, const SVM_Address address, const SVM_Value value);

/**
 * This function modifies the type to an internal type of an address of a memory.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted memory is.
 * \param[in] address The local address to modify.
 * \param[in] type The new type.
 *
 * If the address contained a value, this value is cleared.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 * \throws MEMORY interruption when the address is not defined.
 *
 * \see svm_memory_address_is_defined
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 */
SVM_FUNCTION void svm_memory_address_set_type(const void *svm, const SVM_Kernel kernel, const SVM_Address address, const SVM_Type type);

/** \} */

/** \defgroup pointermemory SVM memory pointers
 * This module contains API functions to handle SVM memory pointers.
 * \{
 */

/**
 * This function extracts all memory zones recursively accessible from a pointer in a memory.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted memory is.
 * \param[in] pointer The pointer locating the first memory zone.
 *
 * \note The returned values are optimised out: the memory zones graph is flatten and contiguous zones are merged to ensure every address is represented only once in the result.
 * \note The input pointer is always placed on the first element of the reply regardless the merge of the accessible zones.
 *
 * Usage example:
 * \code{.c}
 * SVM_Value_Pointer *pointer = svm_memory_pointer_list_accessible(svm,kernel,initial_pointer);
 * for( ; *pointer ; ++pointer) // we don't need to keep pointer on the first pointer
 * {
 * 	svm_memory_scope_set_local(svm,kernel,*pointer);
 * 	SVM_Value_String *alias = svm_memory_pointer_list_alias(svm,kernel,*pointer);
 * 	for( ; *alias ; ++alias) // we don't need to keep alias on the first alias
 * 	{
 * 		svm_memory_scope_set_local_alias(svm,kernel,*alias);
 * 	}
 * }
 * \endcode
 *
 * \returns A NULL terminated array of pointer values locating all accessible memory.
 *
 * \note The returned array is a valid SVM variable: its scope can be changed if needed, and the pointer shall not be freed explicitely.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 *
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 */
SVM_FUNCTION SVM_Value_Pointer* svm_memory_pointer_list_accessible(const void *svm, const SVM_Kernel kernel, const SVM_Value_Pointer pointer);

/**
 * This function extracts all alias having their pointer within a memory zone.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted memory is.
 * \param[in] pointer The memory zone to check.
 *
 * The returned alias have their pointer included in the given memory zone.
 * If an alias has at least one address outside this zone, it will not be retrieved by this API function.
 *
 * \returns A NULL terminated array of string values containing the aliases.
 *
 * \note The returned array is a valid SVM variable: its scope can be changed if needed, and the pointer shall not be freed explicitely.
 *
 * Usage example:
 * \code{.c}
 * SVM_Value_Pointer *pointer = svm_memory_pointer_list_accessible(svm,kernel,initial_pointer);
 * for( ; *pointer ; ++pointer) // we don't need to keep pointer on the first pointer
 * {
 * 	svm_memory_scope_set_local(svm,kernel,*pointer);
 * 	SVM_Value_String *alias = svm_memory_pointer_list_alias(svm,kernel,*pointer);
 * 	for( ; *alias ; ++alias) // we don't need to keep alias on the first alias
 * 	{
 * 		svm_memory_scope_set_local_alias(svm,kernel,*alias);
 * 	}
 * }
 * \endcode
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 *
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 */
SVM_FUNCTION SVM_Value_String* svm_memory_pointer_list_alias(const void *svm, const SVM_Kernel kernel, const SVM_Value_Pointer pointer);

/**
 * This function extracts all alias having their pointer intersecting with a memory zone.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted memory is.
 * \param[in] pointer The memory zone to check.
 *
 * The returned alias have their pointer intersecting in the given memory zone.
 * Each returned alias has at least one address in common with the input pointer, but can have adresses outside the input pointer.
 *
 * \returns A NULL terminated array of string values containing the aliases.
 *
 * \note The returned array is a valid SVM variable: its scope can be changed if needed, and the pointer shall not be freed explicitely.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 *
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 */
SVM_FUNCTION SVM_Value_String* svm_memory_pointer_expand_alias(const void *svm, const SVM_Kernel kernel, const SVM_Value_Pointer pointer);

/**
 * This function checks whether an alias pointer has an intersection with a memory zone.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted memory is.
 * \param[in] pointer The memory zone to check.
 *
 * \returns TRUE when at least an alias has an address in common with the pointer, FALSE otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 *
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 * \see svm_memory_pointer_expand_alias
 */
SVM_FUNCTION SVM_Boolean svm_memory_pointer_has_alias(const void *svm, const SVM_Kernel kernel, const SVM_Value_Pointer pointer);

/**
 * This function creates a memory zone corresponding to the pointer.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted memory is.
 * \param[in] pointer The memory zone to build.
 *
 * \returns The memory zone under the pointer.
 *
 * \throws MEMORY interruption when a memory address inside the pointer is not defined.
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 *
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 * \see svm_memory_zone_new
 * \see svm_memory_allocate
 */
SVM_FUNCTION SVM_Memory_Zone svm_memory_pointer_get_zone(const void *svm, const SVM_Kernel kernel, const SVM_Value_Pointer pointer);

/**
 * This function changes the memory under the pointer to the types of the zone.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted memory is.
 * \param[in] pointer The memory zone to alter.
 * \param[in] zone The types to use.
 *
 * \note The addresses impacted by the function are defined but no longer initialised after this function call.
 *
 * \throws MEMORY interruption when a memory address inside the pointer is not defined.
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 * \throws FAILURE interruption when the zone and the pointer have different sizes.
 *
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 * \see svm_memory_zone_new
 * \see svm_memory_allocate
 */
SVM_FUNCTION void svm_memory_pointer_set_zone(const void *svm, const SVM_Kernel kernel, const SVM_Value_Pointer pointer, const SVM_Memory_Zone zone);

/** \} */

/** \defgroup readmemory SVM memory read
 * This module contains API functions to handle SVM memory read access.
 * \{
 */

/**
 * This function reads a value from a memory.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted memory is.
 * \param[in] address The local address to read from.
 *
 * \note The type of the read value is not checked at all.
 *
 * \returns The value associated to the address.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 * \throws MEMORY interruption when the address is not initialised.
 *
 * \see svm_memory_address_is_initialised
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 */
SVM_FUNCTION SVM_Value svm_memory_read_address(const void *svm, const SVM_Kernel kernel, const SVM_Address address);

/**
 * This function reads a value from a memory having a given type.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted memory is.
 * \param[in] address The local address to read from.
 * \param[in] type The type for type checking.
 *
 * \returns The value associated to the address.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 * \throws MEMORY interruption when the address is not initialised.
 * \throws MEMORY interruption when the address does not contains a value of the expected type.
 *
 * \see svm_memory_address_is_initialised
 * \see svm_memory_address_get_type
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 */
SVM_FUNCTION SVM_Value svm_memory_read_address_type(const void *svm, const SVM_Kernel kernel, const SVM_Address address, const SVM_Type type);

/**
 * This function reads a value from a memory having a given internal type.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted memory is.
 * \param[in] address The local address to read from.
 * \param[in] type The internal type for type checking.
 *
 * \returns The value associated to the address.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 * \throws MEMORY interruption when the address is not initialised.
 * \throws MEMORY interruption when the address does not contains a value of the expected type.
 *
 * \see svm_memory_address_is_initialised
 * \see svm_memory_address_get_type
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 */
SVM_FUNCTION SVM_Value svm_memory_read_address_type_internal(const void *svm, const SVM_Kernel kernel, const SVM_Address address, const SVM_Type_Internal type);

/**
 * This function reads a value from a memory having a given plugin type.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted memory is.
 * \param[in] address The local address to read from.
 * \param[in] type The plugin type name for type checking.
 *
 * \returns The value associated to the address.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 * \throws FAILURE interruption when the type parameter does not identify a plugin type.
 * \throws MEMORY interruption when the address is not initialised.
 * \throws MEMORY interruption when the address does not contains a value of the expected type.
 *
 * \see svm_memory_address_is_initialised
 * \see svm_memory_address_get_type
 * \see svm_memory_address_get_type_name
 * \see svm_plugin_has_type
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 */
SVM_FUNCTION SVM_Value svm_memory_read_address_type_external(const void *svm, const SVM_Kernel kernel, const SVM_Address address, const SVM_Value_PluginEntryPoint type);

/**
 * This function extracts a value from a memory.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted memory is.
 * \param[in] address The local address to read from.
 *
 * \note The type of the read value is not checked at all.
 *
 * \returns The value associated to the address, or a null value when the address is not initialised.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 * \throws MEMORY interruption when the address is not defined.
 *
 * \see svm_memory_address_is_defined
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 */
SVM_FUNCTION SVM_Value svm_memory_extract_address(const void *svm, const SVM_Kernel kernel, const SVM_Address address);

/**
 * This function reads an array of values from a memory.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted memory is.
 * \param[in] pointer The pointer of the zone to read from.
 *
 * \note The types of the read values are not checked at all.
 *
 * \returns The array of values associated to the pointer, terminated by a C/C++ null pointer.
 *
 * \note The returned array is a valid SVM variable: its scope can be changed if needed, and the pointer shall not be freed explicitely.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 * \throws MEMORY interruption when at least one address is not initialised.
 *
 * \see svm_memory_address_is_initialised
 * \see svm_memory_read_address
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 */
SVM_FUNCTION SVM_Value* svm_memory_read_pointer(const void *svm, const SVM_Kernel kernel, const SVM_Value_Pointer pointer);

/**
 * This function reads an array of values from a memory.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted memory is.
 * \param[in] address The address of the zone to read from.
 * \param[in] size The size of the zone to read from.
 *
 * \note The types of the read values are not checked at all.
 *
 * \returns The array of values associated to the pointer, terminated by a C/C++ null pointer.
 *
 * \note The returned array is a valid SVM variable: its scope can be changed if needed, and the pointer shall not be freed explicitely.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 * \throws MEMORY interruption when at least one address is not initialised.
 *
 * \see svm_memory_address_is_initialised
 * \see svm_memory_read_address
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 */
SVM_FUNCTION SVM_Value* svm_memory_read_pointer__raw(const void *svm, const SVM_Kernel kernel, const SVM_Address address, const SVM_Size size);

/**
 * This function reads an array of values from a memory with unique type checking.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted memory is.
 * \param[in] pointer The pointer of the zone to read from.
 * \param[in] type The type for type checking.
 *
 * \returns The array of values associated to the pointer, terminated by a C/C++ null pointer.
 *
 * \note The returned array is a valid SVM variable: its scope can be changed if needed, and the pointer shall not be freed explicitely.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 * \throws MEMORY interruption when at least one address is not initialised.
 * \throws MEMORY interruption when at least one address does not contains a value of the expected type.
 *
 * \see svm_memory_address_is_initialised
 * \see svm_memory_read_address
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 */
SVM_FUNCTION SVM_Value* svm_memory_read_pointer_type(const void *svm, const SVM_Kernel kernel, const SVM_Value_Pointer pointer, const SVM_Type type);

/**
 * This function reads an array of values from a memory with unique type checking.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted memory is.
 * \param[in] address The address of the zone to read from.
 * \param[in] size The size of the zone to read from.
 * \param[in] type The type for type checking.
 *
 * \returns The array of values associated to the pointer, terminated by a C/C++ null pointer.
 *
 * \note The returned array is a valid SVM variable: its scope can be changed if needed, and the pointer shall not be freed explicitely.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 * \throws MEMORY interruption when at least one address is not initialised.
 * \throws MEMORY interruption when at least one address does not contains a value of the expected type.
 *
 * \see svm_memory_address_is_initialised
 * \see svm_memory_read_address
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 */
SVM_FUNCTION SVM_Value* svm_memory_read_pointer_type__raw(const void *svm, const SVM_Kernel kernel, const SVM_Address address, const SVM_Size size, const SVM_Type type);

/**
 * This function reads an array of values from a memory with unique type checking against an internal type.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted memory is.
 * \param[in] pointer The pointer of the zone to read from.
 * \param[in] type The internal type for type checking.
 *
 * \returns The array of values associated to the pointer, terminated by a C/C++ null pointer.
 *
 * \note The returned array is a valid SVM variable: its scope can be changed if needed, and the pointer shall not be freed explicitely.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 * \throws MEMORY interruption when at least one address is not initialised.
 * \throws MEMORY interruption when at least one address does not contains a value of the expected type.
 *
 * \see svm_memory_address_is_initialised
 * \see svm_memory_read_address
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 */
SVM_FUNCTION SVM_Value* svm_memory_read_pointer_type_internal(const void *svm, const SVM_Kernel kernel, const SVM_Value_Pointer pointer, const SVM_Type_Internal type);

/**
 * This function reads an array of values from a memory with unique type checking against an internal type.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted memory is.
 * \param[in] address The address of the zone to read from.
 * \param[in] size The size of the zone to read from.
 * \param[in] type The internal type for type checking.
 *
 * \returns The array of values associated to the pointer, terminated by a C/C++ null pointer.
 *
 * \note The returned array is a valid SVM variable: its scope can be changed if needed, and the pointer shall not be freed explicitely.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 * \throws MEMORY interruption when at least one address is not initialised.
 * \throws MEMORY interruption when at least one address does not contains a value of the expected type.
 *
 * \see svm_memory_address_is_initialised
 * \see svm_memory_read_address
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 */
SVM_FUNCTION SVM_Value* svm_memory_read_pointer_type_internal__raw(const void *svm, const SVM_Kernel kernel, const SVM_Address address, const SVM_Size size, const SVM_Type_Internal type);

/**
 * This function reads an array of values from a memory with unique type checking against an external type.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted memory is.
 * \param[in] pointer The pointer of the zone to read from.
 * \param[in] type The plugin type name for type checking.
 *
 * \returns The array of values associated to the pointer, terminated by a C/C++ null pointer.
 *
 * \note The returned array is a valid SVM variable: its scope can be changed if needed, and the pointer shall not be freed explicitely.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 * \throws MEMORY interruption when at least one address is not initialised.
 * \throws MEMORY interruption when at least one address does not contains a value of the expected type.
 *
 * \see svm_memory_address_is_initialised
 * \see svm_memory_read_address
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 */
SVM_FUNCTION SVM_Value* svm_memory_read_pointer_type_external(const void *svm, const SVM_Kernel kernel, const SVM_Value_Pointer pointer, const SVM_Value_PluginEntryPoint type);

/**
 * This function reads an array of values from a memory with unique type checking against an external type.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted memory is.
 * \param[in] address The address of the zone to read from.
 * \param[in] size The size of the zone to read from.
 * \param[in] type The plugin type name for type checking.
 *
 * \returns The array of values associated to the pointer, terminated by a C/C++ null pointer.
 *
 * \note The returned array is a valid SVM variable: its scope can be changed if needed, and the pointer shall not be freed explicitely.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 * \throws MEMORY interruption when at least one address is not initialised.
 * \throws MEMORY interruption when at least one address does not contains a value of the expected type.
 *
 * \see svm_memory_address_is_initialised
 * \see svm_memory_read_address
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 */
SVM_FUNCTION SVM_Value* svm_memory_read_pointer_type_external__raw(const void *svm, const SVM_Kernel kernel, const SVM_Address address, const SVM_Size size, const SVM_Value_PluginEntryPoint type);

/**
 * This function reads an array of values from a memory with detailed type checking.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted memory is.
 * \param[in] pointer The pointer of the zone to read from.
 * \param[in] zone The zone for type checking.
 *
 * When the type at a read address is compared to the type AUTO in the memory zone, the type checking is disabled for this address.
 *
 * \returns The array of values associated to the pointer, terminated by a C/C++ null pointer.
 *
 * \note The returned array is a valid SVM variable: its scope can be changed if needed, and the pointer shall not be freed explicitely.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 * \throws FAILURE interruption when the zone and the pointer have different sizes.
 * \throws MEMORY interruption when at least one address is not initialised.
 * \throws MEMORY interruption when at least one address does not contains a value of the expected type.
 *
 * \see svm_memory_address_is_initialised
 * \see svm_memory_read_address
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 */
SVM_FUNCTION SVM_Value* svm_memory_read_pointer_zone(const void *svm, const SVM_Kernel kernel, const SVM_Value_Pointer pointer, const SVM_Memory_Zone zone);

/**
 * This function reads an array of values from a memory with detailed type checking.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted memory is.
 * \param[in] address The address of the zone to read from.
 * \param[in] size The size of the zone to read from.
 * \param[in] zone The zone for type checking.
 *
 * \returns The array of values associated to the pointer, terminated by a C/C++ null pointer.
 *
 * \note The returned array is a valid SVM variable: its scope can be changed if needed, and the pointer shall not be freed explicitely.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 * \throws FAILURE interruption when the zone has a size different from the size.
 * \throws MEMORY interruption when at least one address is not initialised.
 * \throws MEMORY interruption when at least one address does not contains a value of the expected type.
 *
 * \see svm_memory_address_is_initialised
 * \see svm_memory_read_address
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 */
SVM_FUNCTION SVM_Value* svm_memory_read_pointer_zone__raw(const void *svm, const SVM_Kernel kernel, const SVM_Address address, const SVM_Size size, const SVM_Memory_Zone zone);

/**
 * This function extracts an array of values from a memory.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted memory is.
 * \param[in] pointer The pointer of the zone to read from.
 *
 * \note The types of the read values are not checked at all.
 *
 * \returns The array of values associated to the pointer with null values for non-initialised addresses, terminated by a C/C++ null pointer.
 *
 * \note The returned array is a valid SVM variable: its scope can be changed if needed, and the pointer shall not be freed explicitely.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 * \throws MEMORY interruption when at least one address is not defined.
 *
 * \see svm_memory_address_is_defined
 * \see svm_memory_read_address
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 */
SVM_FUNCTION SVM_Value* svm_memory_extract_pointer(const void *svm, const SVM_Kernel kernel, const SVM_Value_Pointer pointer);

/**
 * This function reads an array of values from a memory.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted memory is.
 * \param[in] address The address of the zone to read from.
 * \param[in] size The size of the zone to read from.
 *
 * \note The types of the read values are not checked at all.
 *
 * \returns The array of values associated to the pointer with nul values for non-initialised addresses, terminated by a C/C++ null pointer.
 *
 * \note The returned array is a valid SVM variable: its scope can be changed if needed, and the pointer shall not be freed explicitely.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 * \throws MEMORY interruption when at least one address is not defined.
 *
 * \see svm_memory_address_is_defined
 * \see svm_memory_read_address
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 */
SVM_FUNCTION SVM_Value* svm_memory_extract_pointer__raw(const void *svm, const SVM_Kernel kernel, const SVM_Address address, const SVM_Size size);

/** \} */

/** \defgroup writememory SVM memory write
 * This module contains API functions to handle SVM memory write access.
 * \{
 */

/**
 * This function writes a value into a memory.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted memory is.
 * \param[in] address The local address to write to.
 * \param[in] value The value to write.
 *
 * When a null value is written into the memory, the corresponding value is cleared.
 * After the write, the address is still defined but no longer initialised.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 * \throws MEMORY interruption when the address is not defined.
 * \throws MEMORY interruption when the type of the address is incompatible with the value type.
 *
 * \see svm_memory_address_is_defined
 * \see svm_memory_address_is_writable
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 */
SVM_FUNCTION void svm_memory_write_address(const void *svm, const SVM_Kernel kernel, const SVM_Address address, const SVM_Value value);

/**
 * This function writes an array of values into a memory.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted memory is.
 * \param[in] pointer The zone to write to.
 * \param[in] values The values to write.
 *
 * The values are written into the memory only if all types are compatibles. Otherwise, no value is written into the memory.
 *
 * When a null value is written into the memory, the corresponding value is cleared.
 * After the write, the address is still defined but no longer initialised.
 *
 * \warning The array of values shall have a size at least equal to the pointer size. Otherwise, the result is undefined.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 * \throws MEMORY interruption when at least one address is not defined.
 * \throws MEMORY interruption when at least one type of value is incompatible with the corresponding address type.
 *
 * \see svm_memory_address_is_defined
 * \see svm_memory_write_address
 * \see svm_memory_address_is_writable
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 */
SVM_FUNCTION void svm_memory_write_pointer(const void *svm, const SVM_Kernel kernel, const SVM_Value_Pointer pointer, const SVM_Value* values);

/**
 * This function writes an array of values into a memory.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted memory is.
 * \param[in] address The zone address to write to.
 * \param[in] size The zone size to write to.
 * \param[in] values The values to write.
 *
 * The values are written into the memory only if all types are compatibles. Otherwise, no value is written into the memory.
 *
 * When a null value is written into the memory, the corresponding value is cleared.
 * After the write, the address is still defined but no longer initialised.
 *
 * \warning The array of values shall have a size at least equal to the zone size. Otherwise, the result is undefined.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 * \throws MEMORY interruption when at least one address is not defined.
 * \throws MEMORY interruption when at least one type of value is incompatible with the corresponding address type.
 *
 * \see svm_memory_address_is_defined
 * \see svm_memory_write_address
 * \see svm_memory_address_is_writable
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 */
SVM_FUNCTION void svm_memory_write_pointer__raw(const void *svm, const SVM_Kernel kernel, const SVM_Address address, const SVM_Size size, const SVM_Value* values);

/** \} */

/** \defgroup aliasmemory SVM memory aliases
 * This module contains API functions to handle SVM memory aliases.
 * \{
 */

/**
 * This function creates a new alias in a memory from a string value.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted memory is.
 * \param[in] alias The alias to create.
 * \param[in] pointer The memory zone to be identified by the alias.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 * \throws MEMORY interruption when the alias already exists.
 *
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 * \see svm_memory_has_alias
 */
SVM_FUNCTION void svm_memory_add_alias(const void *svm, const SVM_Kernel kernel, const SVM_Value_String alias, const SVM_Value_Pointer pointer);

/**
 * This function creates a new alias in a memory from a SVM_String.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted memory is.
 * \param[in] alias The alias to create.
 * \param[in] pointer The memory zone to be identified by the alias.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 * \throws MEMORY interruption when the alias already exists.
 *
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 * \see svm_memory_has_alias__string
 */
SVM_FUNCTION void svm_memory_add_alias__string(const void *svm, const SVM_Kernel kernel, const SVM_String alias, const SVM_Value_Pointer pointer);

/**
 * This function creates a new alias in a memory from a C nul-terminated value.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted memory is.
 * \param[in] alias The alias to create.
 * \param[in] pointer The memory zone to be identified by the alias.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 * \throws MEMORY interruption when the alias already exists.
 *
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 * \see svm_memory_has_alias__raw
 */
SVM_FUNCTION void svm_memory_add_alias__raw(const void *svm, const SVM_Kernel kernel, const char *alias, const SVM_Value_Pointer pointer);

/**
 * This function checks whether an alias is defined in a memory from a string value.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted memory is.
 * \param[in] alias The alias to check.
 *
 * \returns TRUE if the alias is defined, FALSE otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 *
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 */
SVM_FUNCTION SVM_Boolean svm_memory_has_alias(const void *svm, const SVM_Kernel kernel, const SVM_Value_String alias);

/**
 * This function checks whether an alias is defined in a memory from a SVM_String.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted memory is.
 * \param[in] alias The alias to check.
 *
 * \returns TRUE if the alias is defined, FALSE otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 *
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 */
SVM_FUNCTION SVM_Boolean svm_memory_has_alias__string(const void *svm, const SVM_Kernel kernel, const SVM_String alias);

/**
 * This function checks whether an alias is defined in a memory from a C nul-terminated string.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted memory is.
 * \param[in] alias The alias to check.
 *
 * \returns TRUE if the alias is defined, FALSE otherwise.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 *
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 */
SVM_FUNCTION SVM_Boolean svm_memory_has_alias__raw(const void *svm, const SVM_Kernel kernel, const char *alias);

/**
 * This function extracts the pointer associated to an alias in a memory from a string value.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted memory is.
 * \param[in] alias The alias from which the pointer is extracted .
 *
 * \return The pointer associated to the alias.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 * \throws FAILURE interruption when the alias is not defined.
 *
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 * \see svm_memory_has_alias
 */
SVM_FUNCTION SVM_Value_Pointer svm_memory_alias_get_pointer(const void *svm, const SVM_Kernel kernel, const SVM_Value_String alias);

/**
 * This function extracts the pointer associated to an alias in a memory from a SVM_String.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted memory is.
 * \param[in] alias The alias from which the pointer is extracted .
 *
 * \return The pointer associated to the alias.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 * \throws FAILURE interruption when the alias is not defined.
 *
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 * \see svm_memory_has_alias__string
 */
SVM_FUNCTION SVM_Value_Pointer svm_memory_alias_get_pointer__string(const void *svm, const SVM_Kernel kernel, const SVM_String alias);

/**
 * This function extracts the pointer associated to an alias in a memory from a C nul-terminated string.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted memory is.
 * \param[in] alias The alias from which the pointer is extracted .
 *
 * \return The pointer associated to the alias.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 * \throws FAILURE interruption when the alias is not defined.
 *
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 * \see svm_memory_has_alias__raw
 */
SVM_FUNCTION SVM_Value_Pointer svm_memory_alias_get_pointer__raw(const void *svm, const SVM_Kernel kernel, const char *alias);

/**
 * This function removes an alias from a memory from a string value.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted memory is.
 * \param[in] alias The alias to delete.
 *
 * This API function does nothing if the alias did not exist.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 *
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 */
SVM_FUNCTION void svm_memory_remove_alias(const void *svm, const SVM_Kernel kernel, const SVM_Value_String alias);

/**
 * This function removes an alias from a memory from a SVM_String.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted memory is.
 * \param[in] alias The alias to delete.
 *
 * This API function does nothing if the alias did not exist.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 *
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 */
SVM_FUNCTION void svm_memory_remove_alias__string(const void *svm, const SVM_Kernel kernel, const SVM_String alias);

/**
 * This function removes an alias from a memory from a C nul-terminated string.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted memory is.
 * \param[in] alias The alias to delete.
 *
 * This API function does nothing if the alias did not exist.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 *
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 */
SVM_FUNCTION void svm_memory_remove_alias__raw(const void *svm, const SVM_Kernel kernel, const char *alias);

/** \} */

/** \defgroup operationsmemory SVM memory operations
 * This module contains API functions to handle SVM memory operations.
 * \{
 */

/**
 * This function copies a memory zone to another.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the origin memory is.
 * \param[in] origin The origin memory zone to copy.
 * \param[in] remote The kernel in which the destination memory is.
 * \param[in] target The target memory zone.
 *
 * The two kernels can be equal.
 *
 * Pointers indicating a sub-part of the zone are updated to reflect the new location.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 * \throws FAILURE interruption when the remote kernel is not owned by our process or is not in a process locked by ours.
 * \throws MEMORY interruption when the two memory zones are incompatibles.
 *
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 */
SVM_FUNCTION void svm_memory_copy(const void *svm, const SVM_Kernel kernel, const SVM_Value_Pointer origin, const SVM_Kernel remote, const SVM_Value_Pointer target);

/**
 * This function moves a memory zone to another.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the origin memory is.
 * \param[in] origin The origin memory zone to move.
 * \param[in] remote The kernel in which the destination memory is.
 * \param[in] target The target memory zone.
 *
 * The two kernels can be equal.
 *
 * After the move, the original zone is left non-initialised.
 * Pointers indicating a sub-part of the zone are updated to reflect the new location.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 * \throws FAILURE interruption when the remote kernel is not owned by our process or is not in a process locked by ours.
 * \throws MEMORY interruption when the two memory zones are incompatibles.
 *
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 */
SVM_FUNCTION void svm_memory_move(const void *svm, const SVM_Kernel kernel, const SVM_Value_Pointer origin, const SVM_Kernel remote, const SVM_Value_Pointer target);

/**
 * This function shares a memory zone to another.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the origin memory is.
 * \param[in] origin The origin memory zone to share.
 * \param[in] remote The kernel in which the destination memory is.
 * \param[in] target The target memory zone.
 *
 * The two kernels can be equal.
 *
 * If the two kernels are different, the synchronisation is enabled on the shared memory.
 *
 * After the share, any change to one memory zone will be automatically reflected to the other one.
 * 
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 * \throws FAILURE interruption when the remote kernel is not owned by our process or is not in a process locked by ours.
 * \throws MEMORY interruption when the two memory zones are incompatibles.
 *
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 * \see svm_memory_synchronisation_enable
 * \see svm_memory_synchronisation_disable
 */
SVM_FUNCTION void svm_memory_share(const void *svm, const SVM_Kernel kernel, const SVM_Value_Pointer origin, const SVM_Kernel remote, const SVM_Value_Pointer target);

/**
 * This function translates pointers within a memory zone.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the memory is.
 * \param[in] zone The memory zone where pointers are modified.
 * \param[in] origin The origin memory zone targeted by pointers.
 * \param[in] target The target memory zone in which pointers shall reference.
 *
 * Only non-null pointers are modified within the memory zone. Other values are ignored.
 * All addresses shall be defined within the memory zone.
 *
 * When a pointer is read and is pointing towards the origin memory zone, its address is changed to the same offset within the target memory zone.
 * If the pointer destination exceeds the origin and target zones, the pointer is not modified.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 * \throws MEMORY interruption when a wrong memory access is performed.
 *
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 * \see svm_memory_copy
 * \see svm_memory_move
 * \see svm_memory_share
 */
SVM_FUNCTION void svm_memory_translate(const void *svm, const SVM_Kernel kernel, const SVM_Value_Pointer zone, const SVM_Value_Pointer origin, const SVM_Value_Pointer target);

/**
 * This function increases or decreases an integer in a memory by an integer value.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted memory is.
 * \param[in] address The local address to modify.
 * \param[in] shift The shift value.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 * \throws MEMORY interruption when the address is not defined.
 * \throws MEMORY interruption when the type of the address is not INTEGER.
 *
 * \see svm_memory_address_is_defined
 * \see svm_memory_address_get_type
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 */
SVM_FUNCTION void svm_memory_address_shift(const void *svm, const SVM_Kernel kernel, const SVM_Address address, const SVM_Value_Integer shift);

/**
 * This function increases or decreases an integer in a memory by a raw integer.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted memory is.
 * \param[in] address The local address to modify.
 * \param[in] shift The shift value.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 * \throws MEMORY interruption when the address is not defined.
 * \throws MEMORY interruption when the type of the address is not INTEGER.
 *
 * \see svm_memory_address_is_defined
 * \see svm_memory_address_get_type
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 */
SVM_FUNCTION void svm_memory_address_shift__raw(const void *svm, const SVM_Kernel kernel, const SVM_Address address, signed long int shift);

/** \} */

/** \defgroup synchronisationmemory SVM memory synchronisation
 * This module contains API functions to handle SVM memory synchronisation.
 * \{
 */

/**
 * This function enables the synchronisation on a memory zone.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted memory is.
 * \param[in] pointer The memory zone to protect by synchronisation.
 *
 * \note The synchronisation applies to access to the memory itself, and not access to the value itself. If the value needs access synchronisation, it shall be done manually.
 *
 * The synchronisation mechanism applied to each address of the memory zone is equivalent to SVM_Lock.
 * - When a read to the memory is done from a synchronised address, a read lock acquisition is done,
 * - When a write or deletion of the memory is done on a synchronised address, a write lock acquisition is done.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 *
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 * \see svm_lock_new
 * \see svm_lock_readguard_new
 * \see svm_lock_writeguard_new
 */
SVM_FUNCTION void svm_memory_synchronisation_enable(const void *svm, const SVM_Kernel kernel, const SVM_Value_Pointer pointer);

/**
 * This function disables the synchronisation on a memory zone.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted memory is.
 * \param[in] pointer The memory zone to release from protection by synchronisation.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 *
 * \see svm_process_ownership_lock
 * \see svm_kernel_get_current
 * \see svm_memory_share
 * \see svm_lock_new
 * \see svm_lock_readguard_new
 * \see svm_lock_writeguard_new
 */
SVM_FUNCTION void svm_memory_synchronisation_disable(const void *svm, const SVM_Kernel kernel, const SVM_Value_Pointer pointer);

/** \} */

/** \} */

/** \defgroup debug SVM debugger
 * This module contains some low level access to the debugger.
 *
 * \{
 */

/** \defgroup debuggerbreak SVM debugger break points
 * This module contains API functions to handle SVM debugger break points and notifications.
 * \{
 */

/**
 * This function is a plugin breakpoint with a message from a string value.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted processor is.
 * \param[in] message The breakpoint message.
 *
 * This function allows breakpoints during the instruction execution to see its impact on the machine.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION void svm_debug_break(const void *svm, const SVM_Kernel kernel, const SVM_Value_String message);

/**
 * This function is a plugin breakpoint with a message from a SVM_String.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted processor is.
 * \param[in] message The breakpoint message.
 *
 * This function allows breakpoints during the instruction execution to see its impact on the machine.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION void svm_debug_break__string(const void *svm, const SVM_Kernel kernel, const SVM_String message);

/**
 * This function is a plugin breakpoint with a message from a C nul-terminated string.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted processor is.
 * \param[in] message The breakpoint message.
 *
 * This function allows breakpoints during the instruction execution to see its impact on the machine.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION void svm_debug_break__raw(const void *svm, const SVM_Kernel kernel, const char *message);

/**
 * This function is a plugin notification to the debugger with a message from a string value.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted processor is.
 * \param[in] message The notification message.
 *
 * This function allows to send notification to the debugger.
 *
 * \note The kernel can be a null pointer. In such case, the notification will be sent without any location.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION void svm_debug_notify(const void *svm, const SVM_Kernel kernel, const SVM_Value_String message);

/**
 * This function is a plugin notification to the debugger with a message from a SVM_String.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted processor is.
 * \param[in] message The notification message.
 *
 * This function allows to send notification to the debugger.
 *
 * \note The kernel can be a null pointer. In such case, the notification will be sent without any location.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION void svm_debug_notify__string(const void *svm, const SVM_Kernel kernel, const SVM_String message);

/**
 * This function is a plugin notification to the debugger with a message from a C nul-terminated string.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted processor is.
 * \param[in] message The notification message.
 *
 * This function allows to send notification to the debugger.
 *
 * \note The kernel can be a null pointer. In such case, the notification will be sent without any location.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION void svm_debug_notify__raw(const void *svm, const SVM_Kernel kernel, const char *message);

/** \} */

/**
 * This function allows explicit synchronisation of a value in the debugger.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] value The value to synchronise.
 *
 * This function allows a value to be synchronised in the debugger interface when accessible from a memory.
 *
 * \note This function may trigger a memory access breakpoint.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION void svm_debug_synchronise(const void *svm, const SVM_Value value);

/** \defgroup debuggerinstruction SVM debugger instruction
 * This module contains API functions to handle SVM debugger instruction equivalents.
 * \{
 */

/**
 * This function adds a breakpoint on a processor.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted processor is. 
 * \param[in] symbol The address of the breakpoint.
 *
 * The added breakpoint is equivalent to the one set through the debugger user interface.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 */
SVM_FUNCTION void svm_debug_breakpoint_add_break(const void *svm, const SVM_Kernel kernel, const SVM_Value_Symbol symbol);

/**
 * This function removes a breakpoint on a processor.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted processor is. 
 * \param[in] symbol The address of the breakpoint.
 *
 * The removed breakpoint is equivalent to the one set through the debugger user interface.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 */
SVM_FUNCTION void svm_debug_breakpoint_remove_break(const void *svm, const SVM_Kernel kernel, const SVM_Value_Symbol symbol);

/**
 * This function adds a breakpoint by interruption on a processor.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted processor is. 
 * \param[in] interruption The interruption of the breakpoint.
 *
 * The added breakpoint is the one set by the instruction :debug ADD
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 */
SVM_FUNCTION void svm_debug_breakpoint_add_interruption(const void *svm, const SVM_Kernel kernel, const SVM_Value_Interruption interruption);

/**
 * This function removes a breakpoint by interruption on a processor.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted processor is. 
 * \param[in] interruption The interruption of the breakpoint.
 *
 * The removed breakpoint is the one reset by the instruction :debug REMOVE
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 */
SVM_FUNCTION void svm_debug_breakpoint_remove_interruption(const void *svm, const SVM_Kernel kernel, const SVM_Value_Interruption interruption);

/**
 * This function adds a breakpoint by memory read on a memory.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted memory is. 
 * \param[in] address The memory address of the breakpoint.
 *
 * The added breakpoint is the one set by the instruction :debug ADD READ
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 */
SVM_FUNCTION void svm_debug_breakpoint_add_memoryread(const void *svm, const SVM_Kernel kernel, const SVM_Address address);

/**
 * This function removes a breakpoint by memory read on a memory.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted memory is. 
 * \param[in] address The memory address of the breakpoint.
 *
 * The removed breakpoint is the one reset by the instruction :debug REMOVE READ
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 */
SVM_FUNCTION void svm_debug_breakpoint_remove_memoryread(const void *svm, const SVM_Kernel kernel, const SVM_Address address);

/**
 * This function adds a breakpoint by memory write on a memory.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted memory is. 
 * \param[in] address The memory address of the breakpoint.
 *
 * The added breakpoint is the one set by the instruction :debug ADD WRITE
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 */
SVM_FUNCTION void svm_debug_breakpoint_add_memorywrite(const void *svm, const SVM_Kernel kernel, const SVM_Address address);

/**
 * This function removes a breakpoint by memory write on a memory.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted memory is. 
 * \param[in] address The memory address of the breakpoint.
 *
 * The removed breakpoint is the one reset by the instruction :debug REMOVE WRITE
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 */
SVM_FUNCTION void svm_debug_breakpoint_remove_memorywrite(const void *svm, const SVM_Kernel kernel, const SVM_Address address);

/**
 * This function adds a breakpoint by memory access on a memory.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted memory is. 
 * \param[in] address The memory address of the breakpoint.
 *
 * The added breakpoint is the one set by the instruction :debug ADD ACCESS
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 */
SVM_FUNCTION void svm_debug_breakpoint_add_memoryaccess(const void *svm, const SVM_Kernel kernel, const SVM_Address address);

/**
 * This function removes a breakpoint by memory access on a memory.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted memory is. 
 * \param[in] address The memory address of the breakpoint.
 *
 * The removed breakpoint is the one reset by the instruction :debug REMOVE ACCESS
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 */
SVM_FUNCTION void svm_debug_breakpoint_remove_memoryaccess(const void *svm, const SVM_Kernel kernel, const SVM_Address address);

/**
 * This function adds a breakpoint by memory delete on a memory.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted memory is. 
 * \param[in] address The memory address of the breakpoint.
 *
 * The added breakpoint is the one set by the instruction :debug ADD DELETE
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 */
SVM_FUNCTION void svm_debug_breakpoint_add_memoryfree(const void *svm, const SVM_Kernel kernel, const SVM_Address address);

/**
 * This function removes a breakpoint by memory delete on a memory.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] kernel The kernel in which the targeted memory is. 
 * \param[in] address The memory address of the breakpoint.
 *
 * The removed breakpoint is the one reset by the instruction :debug REMOVE DELETE
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 * \throws FAILURE interruption when the kernel is not owned by our process or is not in a process locked by ours.
 */
SVM_FUNCTION void svm_debug_breakpoint_remove_memoryfree(const void *svm, const SVM_Kernel kernel, const SVM_Address address);

/** \} */

/** \defgroup debuggerform SVM debugger form
 * This module contains API functions to handle SVM debugger forms.
 * \{
 */

/**
 * This function creates a debugger form.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] title The form title.
 *
 * \returns An empty debugger form.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Debug_Form svm_debug_form_new(const void *svm, const char *title);

/**
 * This function appends to a debugger form a checkbox field.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in,out] form The debugger form to update.
 * \param[in] label The field label of this value.
 * \param[in] default_value The initial state of the field value.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION void svm_debug_form_append_checkbox(const void *svm, SVM_Debug_Form form, const char *label, const SVM_Value_Boolean default_value);

/**
 * This function appends to a debugger form a checkbox field.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in,out] form The debugger form to update.
 * \param[in] label The field label of this value.
 * \param[in] default_value The initial state of the field value.
 *
 * The default value can also be a SVM_Boolean.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION void svm_debug_form_append_checkbox__raw(const void *svm, SVM_Debug_Form form, const char *label, const int default_value);

/**
 * This function appends to a debugger form a list of selection field.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in,out] form The debugger form to update.
 * \param[in] label The field label of this value.
 * \param[in] size The number of enumerated values.
 * \param[in] values The enumerated values.
 *
 * The default value is the first one in the array.
 *
 * \warning If the values size is smaller than size, it will result in an undefined behavior.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 *
 * \see svm_value_array_new
 */
SVM_FUNCTION void svm_debug_form_append_selection(const void *svm, SVM_Debug_Form form, const char *label, const SVM_Size size, const SVM_Value *values);

/**
 * This function appends to a debugger form an input field accepting integers.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in,out] form The debugger form to update.
 * \param[in] label The field label of this value.
 * \param[in] default_value The initial value of the field value.
 * \param[in] min_value The minimal value to be accepted in the form.
 * \param[in] max_value The maximal value to be accepted in the form.
 *
 * \note When the maximal value is inferior to the minimal value, the boundary check is disabled.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION void svm_debug_form_append_integer(const void *svm, SVM_Debug_Form form, const char *label, const SVM_Value_Integer default_value, const long long int min_value, const long long int max_value);

/**
 * This function appends to a debugger form an input field accepting integers.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in,out] form The debugger form to update.
 * \param[in] label The field label of this value.
 * \param[in] default_value The initial value of the field value.
 * \param[in] min_value The minimal value to be accepted in the form.
 * \param[in] max_value The maximal value to be accepted in the form.
 *
 * \note When the maximal value is inferior to the minimal value, the boundary check is disabled.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION void svm_debug_form_append_integer__raw(const void *svm, SVM_Debug_Form form, const char *label, const long long int default_value, const long long int min_value, const long long int max_value);

/**
 * This function appends to a debugger form an input field accepting strings.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in,out] form The debugger form to update.
 * \param[in] label The field label of this value.
 * \param[in] default_value The initial value of the field value.
 * \param[in] min_size The minimal value to be accepted in the form.
 * \param[in] max_size The maximal value to be accepted in the form.
 *
 * \note When the maximal value is inferior to the minimal value, the size check is disabled.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION void svm_debug_form_append_string(const void *svm, SVM_Debug_Form form, const char *label, const SVM_Value_String default_value, const SVM_Size min_size, const SVM_Size max_size);

/**
 * This function appends to a debugger form an input field accepting strings.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in,out] form The debugger form to update.
 * \param[in] label The field label of this value.
 * \param[in] default_value The initial value of the field value.
 * \param[in] min_size The minimal value to be accepted in the form.
 * \param[in] max_size The maximal value to be accepted in the form.
 *
 * \note When the maximal value is inferior to the minimal value, the size check is disabled.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION void svm_debug_form_append_string__string(const void *svm, SVM_Debug_Form form, const char *label, const SVM_String default_value, const SVM_Size min_size, const SVM_Size max_size);

/**
 * This function appends to a debugger form an input field accepting strings.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in,out] form The debugger form to update.
 * \param[in] label The field label of this value.
 * \param[in] default_value The initial value of the field value.
 * \param[in] min_size The minimal value to be accepted in the form.
 * \param[in] max_size The maximal value to be accepted in the form.
 *
 * \note When the maximal value is inferior to the minimal value, the size check is disabled.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION void svm_debug_form_append_string__raw(const void *svm, SVM_Debug_Form form, const char *label, const char *default_value, const SVM_Size min_size, const SVM_Size max_size);

/**
 * This function appends to a debugger form an input field accepting long multi-line strings.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in,out] form The debugger form to update.
 * \param[in] label The field label of this value.
 * \param[in] width The field width, in number of characters.
 * \param[in] height The field height, in number of characters.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION void svm_debug_form_append_text(const void *svm, SVM_Debug_Form form, const char *label, const SVM_Size width, const SVM_Size height);

/**
 * This function sends the form to the debugger user interface, and waits for the form submission.
 *
 * \param[in] svm The SVM pointer passed as first argument of the callback function.
 * \param[in] form The debugger form to send.
 *
 * \warning While waiting the user to submit the form, the current process is under a specific breakpoint disallowing execution until the form is submitted.
 *
 * \note When the debugger is not active or when an error happens, a null pointer is returned.
 *
 * \returns An array of values containing the values in the order of the fields in the form. The array has an extra null pointer at its end.
 *
 * \note The returned array is a valid SVM variable: its scope can be changed if needed, and the pointer shall not be freed explicitely.
 *
 * \throws FAILURE interruption when a parameter is incorrect. (Please refer to the main description page of this API.)
 */
SVM_FUNCTION SVM_Value* svm_debug_form_request(const void *svm, const SVM_Debug_Form form);

/** \} */

/** \} */

#ifdef __cplusplus
}
#endif
#endif
