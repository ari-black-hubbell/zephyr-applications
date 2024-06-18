
/* main.c 
 * a sample / test program to practice using the different zephyr modules necessary
 * for the conductor sensor simulator. the code here may be used as reference for
 * future projects.
 */

#include <zephyr/kernel.h>			// zephyr
#include <zephyr/device.h>			// zephyr
#include <stdio.h> 					// hello world
#include <zephyr/random/random.h>	// prf
#include <zephyr/crypto/crypto.h>	// cryptography



/* sample uses of the randomization capabilities provided by zephyr. */
void random_sample(void) {

	/* generate random x-bit values (can be positive or negative).
	 * not cryptographically secure.
	 */
	uint8_t  val8 	= sys_rand8_get();	// 8-bit value
	uint16_t val16 	= sys_rand16_get();	// 16-bit value
	uint32_t val32 	= sys_rand32_get();	// 32-bit value
	uint64_t val64 	= sys_rand64_get(); // 64-bit value

	// print random values
	printk("randomly generated values: \n	\
	  8-bit 	--> %d 		\n	\
	  16-bit 	--> %d 		\n	\
	  32-bit	--> %d 		\n	\
	  64-bit	--> %lld	\n", 
	  val8, val16, val32, val64);
}

/* sample uses of the hash capabilities provided by zephyr. */
void hash_sample(void) {
	
	// define context structure
	struct hash_ctx ctx;								// holds one time parameters

	// add some flags (crypto.h)
	// int caps = crypto_query_hwcaps(dev);				// query hw supported flags
	ctx.flags = CAP_SYNC_OPS | CAP_SEPARATE_IO_BUFS; 

	// define dev structure for driver instance
	const struct device *dev = device_get_binding(CONFIG_CRYPTO_MBEDTLS_SHIM_DRV_NAME);

	// begin hash session (sha224, 256, 384, 512)
	int ret = hash_begin_session(dev, &ctx, CRYPTO_HASH_ALGO_SHA256);	// sha256
	if (ret == 0) {printk("initialized sha256 session\n");}
	else {printk("failed to initialize sha256 session\n");}

	// define packet (input/output) structure
	// struct hash_pkt pkt;
	uint8_t hash_val[] = {0xbe};
	uint8_t out_buf[32] = {0};			// init output buffer
	struct hash_pkt pkt = {				// init pkt to hold input and output hash data
		.in_buf = hash_val,				// start address of input buffer
		.in_len = sizeof(hash_val),		// bytes to be hashed
		.out_buf = out_buf,				// start of output buffer, can be NULL for in place ops
	};

	// perform hash function	(may not produce the correct hash?)
	ret = hash_compute(&ctx, &pkt);
	if (ret == 0) {printk("successfully computed hash\n");}
	else {printk("failed to compute hash\n");}

	// output hash in hex (has 4 extra characters at the end)
	char* cur = pkt.out_buf;
	for ( ; *cur != '\0'; ++cur) {printk("%02x", *cur);} printk("\n");
}

/* sample uses of the cipher capabilities provided by zephyr. */
void cipher_sample(void) {

	// define context structure and flags
	struct cipher_ctx ctx;
	ctx.flags = CAP_SYNC_OPS | CAP_SEPARATE_IO_BUFS; 

	// define device structure for driver instance
	const struct device *dev = device_get_binding(CONFIG_CRYPTO_MBEDTLS_SHIM_DRV_NAME);

	// begin session (device, context, algorithm, mode, encrypt/decrypt)
	int ret = cipher_begin_session(dev, &ctx, CRYPTO_CIPHER_ALGO_AES, CRYPTO_CIPHER_MODE_CCM, CRYPTO_CIPHER_OP_ENCRYPT);
	if (ret == 0) {printk("initialized AES-CCM session\n");}
	else {printk("failed to initialize AES-CCM session\n");}

	// NOTE: you can do BT cryptography using bt_ccm_encrypt/decrypt
	// with zephyr's bt cryptography API


}

int main(void) {
    // hello world sample code
	printk("Hello World! %s\n", CONFIG_BOARD_TARGET);

	// random module
	printk("------------------------------------------------------------------\n");
	random_sample();
	printk("------------------------------------------------------------------\n");

	// hash module
	printk("------------------------------------------------------------------\n");
	hash_sample();
	printk("------------------------------------------------------------------\n");

	// cipher sample
	printk("------------------------------------------------------------------\n");
	cipher_sample();
	printk("------------------------------------------------------------------\n");

	return 0;
}
