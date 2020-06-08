/*
 * This file is part of trust|me
 * Copyright(c) 2013 - 2020 Fraunhofer AISEC
 * Fraunhofer-Gesellschaft zur Förderung der angewandten Forschung e.V.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2 (GPL 2), as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GPL 2 license for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, see <http://www.gnu.org/licenses/>
 *
 * The full GNU General Public License is included in this distribution in
 * the file called "COPYING".
 *
 * Contact Information:
 * Fraunhofer AISEC <trustme@aisec.fraunhofer.de>
 */

#ifndef TOKEN_H
#define TOKEN_H

#include "softtoken.h"
#include "usbtoken.h"

#include "common/uuid.h"

#include <stdbool.h>
#include <stddef.h>

#define STOKEN_DEFAULT_PASS "trustme"

/**
 *  Generic token type
 */
typedef struct scd_token scd_token_t;

/**
 * Internal data if the generic scd token.
 */
typedef struct scd_token_data scd_token_data_t;

/**
 * Choice of supported token types.
 * Must be kept in sync with scd.proto
 */
typedef enum scd_tokentype { NONE, DEVICE, USB } scd_tokentype_t;

/**
 * Data used by the constructor scd_token_new
 */
typedef struct token_constr_data {
	scd_tokentype_t type;
	char *uuid;

	union {
		const char *softtoken_dir;
		const char *usbtoken_serial;
	} str;
} token_constr_data_t;

/**
 *  generic scd_token.
 */
struct scd_token {
	scd_token_data_t *token_data;

	int (*lock)(scd_token_t *token);
	int (*unlock)(scd_token_t *token, char *passwd, unsigned char *pairing_secret,
		      size_t pairing_sec_len);

	bool (*is_locked)(scd_token_t *token);
	bool (*is_locked_till_reboot)(scd_token_t *token);

	int (*wrap_key)(scd_token_t *token, char *label, unsigned char *plain_key,
			size_t plain_key_len, unsigned char **wrapped_key, int *wrapped_key_len);

	int (*unwrap_key)(scd_token_t *token, char *label, unsigned char *wrapped_key,
			  size_t wrapped_key_len, unsigned char **plain_key, int *plain_key_len);

	int (*change_passphrase)(scd_token_t *token, const char *oldpass, const char *newpass,
				 unsigned char *pairing_secret, size_t pairing_sec_len,
				 bool is_provisioning);
};

/**
 * returns the token's type.
 */
scd_tokentype_t
token_get_type(scd_token_t *token);

/**
 * returns the token's uuid.
 */
uuid_t *
token_get_uuid(scd_token_t *token);

scd_token_t *
token_new(token_constr_data_t *constr_data);

void
token_free(scd_token_t *token);

#endif /* TOKEN_H */