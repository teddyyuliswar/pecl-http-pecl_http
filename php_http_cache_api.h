/*
   +----------------------------------------------------------------------+
   | PECL :: http                                                         |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.0 of the PHP license, that  |
   | is bundled with this package in the file LICENSE, and is available   |
   | through the world-wide-web at http://www.php.net/license/3_0.txt.    |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
   | Copyright (c) 2004-2005 Michael Wallner <mike@php.net>               |
   +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifndef PHP_HTTP_CACHE_API_H
#define PHP_HTTP_CACHE_API_H

#include "php_http_std_defs.h"
#include "php_http_send_api.h"

#define http_etag(p, l, m) _http_etag((p), (l), (m) TSRMLS_CC)
PHP_HTTP_API char *_http_etag(const void *data_ptr, size_t data_len, http_send_mode data_mode TSRMLS_DC);

#define http_lmod(p, m) _http_lmod((p), (m) TSRMLS_CC)
PHP_HTTP_API time_t _http_lmod(const void *data_ptr, http_send_mode data_mode TSRMLS_DC);

#define http_modified_match(entry, modified) _http_modified_match_ex((entry), (modified), 1 TSRMLS_CC)
#define http_modified_match_ex(entry, modified, ep) _http_modified_match_ex((entry), (modified), (ep) TSRMLS_CC)
PHP_HTTP_API zend_bool _http_modified_match_ex(const char *entry, time_t t, zend_bool enforce_presence TSRMLS_DC);

#define http_etag_match(entry, etag) _http_etag_match_ex((entry), (etag), 1 TSRMLS_CC)
#define http_etag_match_ex(entry, etag, ep) _http_etag_match_ex((entry), (etag), (ep) TSRMLS_CC)
PHP_HTTP_API zend_bool _http_etag_match_ex(const char *entry, const char *etag, zend_bool enforce_presence TSRMLS_DC);

#define http_cache_last_modified(l, s, cc, ccl) _http_cache_last_modified((l), (s), (cc), (ccl) TSRMLS_CC)
PHP_HTTP_API STATUS _http_cache_last_modified(time_t last_modified, time_t send_modified, const char *cache_control, size_t cc_len TSRMLS_DC);

#define http_cache_etag(e, el, cc, ccl) _http_cache_etag((e), (el), (cc), (ccl) TSRMLS_CC)
PHP_HTTP_API STATUS _http_cache_etag(const char *etag, size_t etag_len, const char *cache_control, size_t cc_len TSRMLS_DC);

#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
