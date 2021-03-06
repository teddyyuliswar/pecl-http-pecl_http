/*
    +--------------------------------------------------------------------+
    | PECL :: http                                                       |
    +--------------------------------------------------------------------+
    | Redistribution and use in source and binary forms, with or without |
    | modification, are permitted provided that the conditions mentioned |
    | in the accompanying LICENSE file are met.                          |
    +--------------------------------------------------------------------+
    | Copyright (c) 2004-2014, Michael Wallner <mike@php.net>            |
    +--------------------------------------------------------------------+
*/

#ifndef PHP_HTTP_MISC_H
#define PHP_HTTP_MISC_H

/* DEFAULTS */

/* DATE FORMAT RFC1123 */
#define PHP_HTTP_DATE_FORMAT "D, d M Y H:i:s \\G\\M\\T"

/* CR LF */
#define PHP_HTTP_CRLF "\r\n"

/* def URL arg separator */
#define PHP_HTTP_URL_ARGSEP "&"

/* send buffer size */
#define PHP_HTTP_SENDBUF_SIZE 40960

/* SLEEP */

#define PHP_HTTP_DIFFSEC (0.001)
#define PHP_HTTP_MLLISEC (1000)
#define PHP_HTTP_MCROSEC (1000 * 1000)
#define PHP_HTTP_NANOSEC (1000 * 1000 * 1000)
#define PHP_HTTP_MSEC(s) ((long)(s * PHP_HTTP_MLLISEC))
#define PHP_HTTP_USEC(s) ((long)(s * PHP_HTTP_MCROSEC))
#define PHP_HTTP_NSEC(s) ((long)(s * PHP_HTTP_NANOSEC))

PHP_HTTP_API void php_http_sleep(double s);

/* STRING UTILITIES */

#ifndef PTR_SET
#	define PTR_SET(STR, SET) \
	{ \
		PTR_FREE(STR); \
		STR = SET; \
	}
#endif

#define STR_PTR(s) (s?s:"")

#define lenof(S) (sizeof(S) - 1)

#define PHP_HTTP_MATCH_LOOSE	0
#define PHP_HTTP_MATCH_CASE		0x01
#define PHP_HTTP_MATCH_WORD		0x10
#define PHP_HTTP_MATCH_FULL		0x20
#define PHP_HTTP_MATCH_STRICT	(PHP_HTTP_MATCH_CASE|PHP_HTTP_MATCH_FULL)

int php_http_match(const char *haystack, const char *needle, int flags);
char *php_http_pretty_key(register char *key, size_t key_len, zend_bool uctitle, zend_bool xhyphen);
size_t php_http_boundary(char *buf, size_t len TSRMLS_DC);
int php_http_select_str(const char *cmp, int argc, ...);

/* See "A Reusable Duff Device" By Ralf Holly, August 01, 2005 */
#define PHP_HTTP_DUFF_BREAK() times_=1
#define PHP_HTTP_DUFF(c, a) do { \
	size_t count_ = (c); \
	size_t times_ = (count_ + 7) >> 3; \
	switch (count_ & 7){ \
		case 0:	do { \
			a; \
		case 7: \
			a; \
		case 6: \
			a; \
		case 5: \
			a; \
		case 4: \
			a; \
		case 3: \
			a; \
		case 2: \
			a; \
		case 1: \
			a; \
					} while (--times_ > 0); \
	} \
} while (0)

static inline const char *php_http_locate_str(register const char *h, size_t h_len, const char *n, size_t n_len)
{
	if (!n_len || !h_len || h_len < n_len) {
		return NULL;
	}

	PHP_HTTP_DUFF(h_len - n_len + 1,
		if (*h == *n && !strncmp(h + 1, n + 1, n_len - 1)) {
			return h;
		}
		++h;
	);

	return NULL;
}

static inline const char *php_http_locate_eol(const char *line, int *eol_len)
{
	const char *eol = strpbrk(line, "\r\n");

	if (eol_len) {
		*eol_len = eol ? ((eol[0] == '\r' && eol[1] == '\n') ? 2 : 1) : 0;
	}
	return eol;
}

static inline const char *php_http_locate_bin_eol(const char *bin, size_t len, int *eol_len)
{
	register const char *eol = bin;

	if (len > 0) {
		PHP_HTTP_DUFF(len,
			if (*eol == '\r' || *eol == '\n') {
				if (eol_len) {
					*eol_len = ((eol[0] == '\r' && eol[1] == '\n') ? 2 : 1);
				}
				return eol;
			}
			++eol;
		);
	}
	return NULL;
}

/* ZEND */

#if PHP_VERSION_ID < 50400
#	define object_properties_init(o, ce) zend_hash_copy(((zend_object *) o)->properties, &(ce->default_properties), (copy_ctor_func_t) zval_add_ref, NULL, sizeof(zval*))
#	define PHP_HTTP_ZEND_LITERAL_DC 
#	define PHP_HTTP_ZEND_LITERAL_CC
#	define PHP_HTTP_ZEND_LITERAL_CCN
#	define ZVAL_COPY_VALUE(zv, arr) do { \
		(zv)->value = (arr)->value; \
		Z_TYPE_P(zv) = Z_TYPE_P(arr); \
	} while (0)
#else
#	define PHP_HTTP_ZEND_LITERAL_DC , const zend_literal *literal_key
#	define PHP_HTTP_ZEND_LITERAL_CC , (literal_key)
#	define PHP_HTTP_ZEND_LITERAL_CCN , NULL
#endif

#if PHP_VERSION_ID < 50700
#	define z_is_true zend_is_true
#else
#	define z_is_true(z) zend_is_true(z TSRMLS_CC)
#endif

#define INIT_PZVAL_ARRAY(zv, ht) \
	{ \
		INIT_PZVAL((zv)); \
		Z_TYPE_P(zv) = IS_ARRAY; \
		Z_ARRVAL_P(zv) = (ht); \
	}

static inline zval *php_http_zconv(int type, zval *z)
{
	switch (type) {
		case IS_NULL:	convert_to_null_ex(&z);		break;
		case IS_BOOL:	convert_to_boolean_ex(&z);	break;
		case IS_LONG:	convert_to_long_ex(&z);		break;
		case IS_DOUBLE:	convert_to_double_ex(&z);	break;
		case IS_STRING:	convert_to_string_ex(&z);	break;
		case IS_ARRAY:	convert_to_array_ex(&z);	break;
		case IS_OBJECT:	convert_to_object_ex(&z);	break;
	}
	return z;
}

static inline zval *php_http_ztyp(int type, zval *z)
{
	SEPARATE_ARG_IF_REF(z);
	return (Z_TYPE_P(z) == type) ? z : php_http_zconv(type, z);
}

static inline zval *php_http_zsep(zend_bool add_ref, int type, zval *z)
{
	if (add_ref) {
		Z_ADDREF_P(z);
	}
	if (Z_TYPE_P(z) != type) {
		return php_http_zconv(type, z);
	} else {
		SEPARATE_ZVAL_IF_NOT_REF(&z);
		return z;
	}
}

static inline STATUS php_http_ini_entry(const char *name_str, size_t name_len, const char **value_str, size_t *value_len, zend_bool orig TSRMLS_DC)
{
	zend_ini_entry *ini_entry;

	if (SUCCESS == zend_hash_find(EG(ini_directives), name_str, name_len + 1, (void *) &ini_entry)) {
		if (orig && ini_entry->modified) {
			*value_str = ini_entry->orig_value;
			*value_len = (size_t) ini_entry->orig_value_length;
		} else {
			*value_str = ini_entry->value;
			*value_len = (size_t) ini_entry->value_length;
		}
		return SUCCESS;
	}
	return FAILURE;
}

/* return object(values) */
#define RETVAL_OBJECT(o, addref) \
	RETVAL_OBJVAL((o)->value.obj, addref)
#define RETURN_OBJECT(o, addref) \
	RETVAL_OBJECT(o, addref); \
	return
#define RETVAL_OBJVAL(ov, addref) \
	ZVAL_OBJVAL(return_value, ov, addref)
#define RETURN_OBJVAL(ov, addref) \
	RETVAL_OBJVAL(ov, addref); \
	return
#define ZVAL_OBJVAL(zv, ov, addref) \
	(zv)->type = IS_OBJECT; \
	(zv)->value.obj = (ov);\
	if (addref && Z_OBJ_HT_P(zv)->add_ref) { \
		Z_OBJ_HT_P(zv)->add_ref((zv) TSRMLS_CC); \
	}

#define Z_OBJ_DELREF(z) \
	if (Z_OBJ_HT(z)->del_ref) { \
		Z_OBJ_HT(z)->del_ref(&(z) TSRMLS_CC); \
	}
#define Z_OBJ_ADDREF(z) \
	if (Z_OBJ_HT(z)->add_ref) { \
		Z_OBJ_HT(z)->add_ref(&(z) TSRMLS_CC); \
	}
#define Z_OBJ_DELREF_P(z) \
	if (Z_OBJ_HT_P(z)->del_ref) { \
		Z_OBJ_HT_P(z)->del_ref((z) TSRMLS_CC); \
	}
#define Z_OBJ_ADDREF_P(z) \
	if (Z_OBJ_HT_P(z)->add_ref) { \
		Z_OBJ_HT_P(z)->add_ref((z) TSRMLS_CC); \
	}
#define Z_OBJ_DELREF_PP(z) \
	if (Z_OBJ_HT_PP(z)->del_ref) { \
		Z_OBJ_HT_PP(z)->del_ref(*(z) TSRMLS_CC); \
	}
#define Z_OBJ_ADDREF_PP(z) \
	if (Z_OBJ_HT_PP(z)->add_ref) { \
		Z_OBJ_HT_PP(z)->add_ref(*(z) TSRMLS_CC); \
	}

#define EMPTY_FUNCTION_ENTRY {NULL, NULL, NULL, 0, 0}

#define PHP_MINIT_CALL(func) PHP_MINIT(func)(INIT_FUNC_ARGS_PASSTHRU)
#define PHP_RINIT_CALL(func) PHP_RINIT(func)(INIT_FUNC_ARGS_PASSTHRU)
#define PHP_MSHUTDOWN_CALL(func) PHP_MSHUTDOWN(func)(SHUTDOWN_FUNC_ARGS_PASSTHRU)
#define PHP_RSHUTDOWN_CALL(func) PHP_RSHUTDOWN(func)(SHUTDOWN_FUNC_ARGS_PASSTHRU)

/* ARRAYS */

#ifndef HASH_KEY_NON_EXISTENT
#	define HASH_KEY_NON_EXISTENT HASH_KEY_NON_EXISTANT
#endif

PHP_HTTP_API unsigned php_http_array_list(HashTable *ht TSRMLS_DC, unsigned argc, ...);

typedef struct php_http_array_hashkey {
	char *str;
	uint len;
	ulong num;
	uint dup:1;
	uint type:31;
} php_http_array_hashkey_t;
#define php_http_array_hashkey_init(dup) {NULL, 0, 0, (dup), 0}

static inline void php_http_array_hashkey_stringify(php_http_array_hashkey_t *key)
{
	if (key->type != HASH_KEY_IS_STRING) {
		key->len = spprintf(&key->str, 0, "%lu", key->num) + 1;
	}
}

static inline void php_http_array_hashkey_stringfree(php_http_array_hashkey_t *key)
{
	if (key->type != HASH_KEY_IS_STRING || key->dup) {
		PTR_FREE(key->str);
	}
}

#define FOREACH_VAL(pos, array, val) FOREACH_HASH_VAL(pos, HASH_OF(array), val)
#define FOREACH_HASH_VAL(pos, hash, val) \
	for (	zend_hash_internal_pointer_reset_ex(hash, &pos); \
			zend_hash_get_current_data_ex(hash, (void *) &val, &pos) == SUCCESS; \
			zend_hash_move_forward_ex(hash, &pos))

#define FOREACH_KEY(pos, array, key) FOREACH_HASH_KEY(pos, HASH_OF(array), key)
#define FOREACH_HASH_KEY(pos, hash, _key) \
	for (	zend_hash_internal_pointer_reset_ex(hash, &pos); \
			((_key).type = zend_hash_get_current_key_ex(hash, &(_key).str, &(_key).len, &(_key).num, (zend_bool) (_key).dup, &pos)) != HASH_KEY_NON_EXISTANT; \
			zend_hash_move_forward_ex(hash, &pos)) \

#define FOREACH_KEYVAL(pos, array, key, val) FOREACH_HASH_KEYVAL(pos, HASH_OF(array), key, val)
#define FOREACH_HASH_KEYVAL(pos, hash, _key, val) \
	for (	zend_hash_internal_pointer_reset_ex(hash, &pos); \
			((_key).type = zend_hash_get_current_key_ex(hash, &(_key).str, &(_key).len, &(_key).num, (zend_bool) (_key).dup, &pos)) != HASH_KEY_NON_EXISTANT && \
			zend_hash_get_current_data_ex(hash, (void *) &val, &pos) == SUCCESS; \
			zend_hash_move_forward_ex(hash, &pos))

#define array_copy(src, dst) zend_hash_copy(dst, src, (copy_ctor_func_t) zval_add_ref, NULL, sizeof(zval *))
#define array_copy_strings(src, dst) zend_hash_copy(dst, src, php_http_array_copy_strings, NULL, sizeof(zval *))
#define ARRAY_JOIN_STRONLY   0x01
#define ARRAY_JOIN_PRETTIFY  0x02
#define ARRAY_JOIN_STRINGIFY 0x04
#define array_join(src, dst, append, flags) zend_hash_apply_with_arguments(src TSRMLS_CC, (append)?php_http_array_apply_append_func:php_http_array_apply_merge_func, 2, dst, (int)flags)

void php_http_array_copy_strings(void *zpp);
int php_http_array_apply_append_func(void *pDest TSRMLS_DC, int num_args, va_list args, zend_hash_key *hash_key);
int php_http_array_apply_merge_func(void *pDest TSRMLS_DC, int num_args, va_list args, zend_hash_key *hash_key);

/* PASS CALLBACK */

typedef size_t (*php_http_pass_callback_t)(void *cb_arg, const char *str, size_t len);
typedef size_t (*php_http_pass_php_http_buffer_callback_t)(void *cb_arg, php_http_buffer_t *str);
typedef size_t (*php_http_pass_format_callback_t)(void *cb_arg, const char *fmt, ...);

typedef struct php_http_pass_fcall_arg {
	zval *fcz;
	zend_fcall_info fci;
	zend_fcall_info_cache fcc;
#ifdef ZTS
	void ***ts;
#endif
} php_http_pass_fcall_arg_t;

PHP_HTTP_API size_t php_http_pass_fcall_callback(void *cb_arg, const char *str, size_t len);

#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
