/*
 * i2c.cpp
 */

#include "stm32_unict_lib.h"

#define ACK         0
#define NOT_ACK     1

#define i2c_stop() {                           \
        m_I2C->CR1 |= I2C_CR1_STOP;            \
    }

#define i2c_check_error() {                                             \
        if ((SR1 & (I2C_SR1_TIMEOUT | I2C_SR1_OVR | I2C_SR1_AF | I2C_SR1_ARLO | I2C_SR1_BERR)) != 0) { \
            i2c_stop();                                                 \
            return SR1;                                                 \
        }                                                               \
    }

#define i2c_wait_free() {                                       \
        uint16_t SR1;                                           \
        uint16_t SR2;                                           \
        do {                                                    \
            SR1 = m_I2C->SR1;                                   \
            SR2 = m_I2C->SR2;                                   \
            i2c_check_error();                                  \
        }                                                       \
	while ((SR2 & I2C_SR2_BUSY) != 0) ;                     \
    }

#define i2c_start() {                                           \
        uint16_t SR1;                                           \
        uint16_t SR2;                                           \
        m_I2C->CR1 |= I2C_CR1_START;                            \
        do {                                                    \
            SR1 = m_I2C->SR1;                                   \
            SR2 = m_I2C->SR2;                                   \
            i2c_check_error();                                  \
        }                                                       \
	while ((SR1 & I2C_SR1_SB) == 0) ;                       \
    }

#define i2c_address_write(data){                                \
        uint16_t SR1;                                           \
        uint16_t SR2;                                           \
        m_I2C->DR = (data);                                     \
        do {                                                    \
            SR1 = m_I2C->SR1;                                   \
            SR2 = m_I2C->SR2;                                   \
            i2c_check_error();                                  \
        }                                                       \
	while ((SR1 & I2C_SR1_ADDR) == 0) ;                     \
    }

#define i2c_data_write(data){                                   \
        uint16_t SR1;                                           \
        uint16_t SR2;                                           \
        m_I2C->DR = (data);                                     \
        do {                                                    \
            SR1 = m_I2C->SR1;                                   \
            SR2 = m_I2C->SR2;                                   \
            i2c_check_error();                                  \
        }                                                       \
	while ((SR1 & I2C_SR1_TXE) == 0) ;                      \
    }

#define i2c_write_flush() {                                     \
        uint16_t SR1;                                           \
        uint16_t SR2;                                           \
        do {                                                    \
            SR1 = m_I2C->SR1;                                   \
            SR2 = m_I2C->SR2;                                   \
            i2c_check_error();                                  \
        }                                                       \
	while ((SR1 & I2C_SR1_BTF) == 0) ;                      \
    }

#define i2c_restart()     i2c_start()



#define i2c_read(data){                                          \
        uint16_t SR1;                                            \
        uint16_t SR2;                                            \
        do {                                                     \
            SR1 = m_I2C->SR1;                                    \
            SR2 = m_I2C->SR2;                                    \
            i2c_check_error();                                   \
        }                                                        \
	while ((SR1 & I2C_SR1_RXNE) == 0) ;                      \
        (data) = m_I2C->DR;                                      \
}

#define i2c_next_ack()      m_I2C->CR1 |= I2C_CR1_ACK
#define i2c_next_no_ack()   m_I2C->CR1 &= ~I2C_CR1_ACK


#define I2C_DUTYCYCLE_2                 ((uint32_t)0x00000000)
#define I2C_DUTYCYCLE_16_9              I2C_CCR_DUTY

#define I2C_FREQRANGE(__PCLK__)                            ((__PCLK__)/1000000)
#define I2C_RISE_TIME(__FREQRANGE__, __SPEED__)            (((__SPEED__) <= 100000) ? ((__FREQRANGE__) + 1) : ((((__FREQRANGE__) * 300) / 1000) + 1))
#define I2C_SPEED_STANDARD(__PCLK__, __SPEED__)            (((((__PCLK__)/((__SPEED__) << 1)) & I2C_CCR_CCR) < 4)? 4:((__PCLK__) / ((__SPEED__) << 1)))
#define I2C_SPEED_FAST(__PCLK__, __SPEED__, __DUTYCYCLE__) (((__DUTYCYCLE__) == I2C_DUTYCYCLE_2)? ((__PCLK__) / ((__SPEED__) * 3)) : (((__PCLK__) / ((__SPEED__) * 25)) | I2C_DUTYCYCLE_16_9))
#define I2C_SPEED(__PCLK__, __SPEED__, __DUTYCYCLE__)      (((__SPEED__) <= 100000)? (I2C_SPEED_STANDARD((__PCLK__), (__SPEED__))) : \
                                                                  ((I2C_SPEED_FAST((__PCLK__), (__SPEED__), (__DUTYCYCLE__)) & I2C_CCR_CCR) == 0)? 1 : \
                                                                  ((I2C_SPEED_FAST((__PCLK__), (__SPEED__), (__DUTYCYCLE__))) | I2C_CCR_FS))


void I2C_init(I2C_TypeDef * m_I2C, int m_speed)
{
	if (m_I2C == I2C1) {
		/* Enable I2C1 clock */
		RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;

		/* Enable GPIOB clock */
		RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

		// PB8 is I2C_SCL
		GPIO_ALTERNATE(GPIOB, 8);
		GPIO_SPEED_HIGH(GPIOB, 8);
		GPIO_OPEN_DRAIN(GPIOB, 8);
		GPIO_NO_PUPD(GPIOB, 8);
		GPIO_AFH(GPIOB, 8, AF_I2C1_3);

		// PB9 is I2C_SDA
		GPIO_ALTERNATE(GPIOB, 9);
		GPIO_SPEED_HIGH(GPIOB, 9);
		GPIO_OPEN_DRAIN(GPIOB, 9);
		GPIO_NO_PUPD(GPIOB, 9);
		GPIO_AFH(GPIOB, 9, AF_I2C1_3);
	}

    uint32_t clock_freq = 42000000;
    uint32_t freqrange = I2C_FREQRANGE(clock_freq);

    m_I2C->CR1 &= ~I2C_CR1_PE;

    // combine all to set
    m_I2C->CR2 = freqrange; // set PCLK1 is set to 42MHz, all other bits to 0 (no interrupt generation)
    m_I2C->TRISE = I2C_RISE_TIME(freqrange, m_speed);
    m_I2C->CCR = I2C_SPEED(clock_freq, m_speed, I2C_DUTYCYCLE_16_9); // Fast + Duty

    m_I2C->CR1 |= I2C_CR1_PE;
}

int I2C_read_register(I2C_TypeDef * m_I2C, short address, short location, short * data)
{
    i2c_start();
    i2c_address_write(address);
    i2c_data_write(location);
    i2c_write_flush();
    i2c_restart();

    i2c_address_write(address | 0x01);
    i2c_next_no_ack();
    i2c_read( *data );
    i2c_stop();

    return 0;
}

int I2C_read_16_bit_register(I2C_TypeDef * m_I2C, short address, short location, int * data)
{
    int h, l;

    i2c_start();
    i2c_address_write(address);
    i2c_data_write(location);
    i2c_write_flush();
    i2c_restart();

    i2c_address_write(address | 0x01);
    i2c_next_no_ack();
    i2c_read( h );
    i2c_next_ack();
    i2c_read( l );
    i2c_stop();

    *data = (h << 8) | l;

    return 0;
}

int I2C_read_buffer(I2C_TypeDef * m_I2C, short address, short location, unsigned char * data, int data_len)
{
    int i;

    i2c_start();
    i2c_address_write(address);
    i2c_data_write(location);
    i2c_write_flush();

    i2c_restart();
    i2c_address_write(address | 0x01);
    if (data_len == 1)
        i2c_next_no_ack();
    else
        i2c_next_ack();

    for (i = 0; i < data_len ;i++) {

        i2c_read(data[i]);
        if (i < (data_len - 2)) {
            i2c_next_ack();
        }
        else {
            i2c_next_no_ack();
        }
    }

    i2c_stop();

    return 0;
}

int I2C_write_register(I2C_TypeDef * m_I2C, short address,short location,short data)
{
    i2c_wait_free();

    i2c_start();
    i2c_address_write(address);
    i2c_data_write(location);
    i2c_data_write(data);
    i2c_write_flush();
    i2c_stop();

    return 0;
}

int I2C_write_buffer(I2C_TypeDef * m_I2C, short address, short location, unsigned char * data, int len)
{
    i2c_start();
    i2c_address_write(address);
    i2c_data_write(location);
    while (len > 0) {
        i2c_data_write(*data);
        ++data;
        --len;
    }
    i2c_write_flush();
    i2c_stop();

    return 0;
}

int I2C_write_buffer_raw(I2C_TypeDef * m_I2C, short address, unsigned char * data, int len)
{
    i2c_start();
    i2c_address_write(address);
    while (len > 0) {
        i2c_data_write(*data);
        ++data;
        --len;
    }
    i2c_write_flush();
    i2c_stop();

    return 0;
}

int I2C_read_buffer_raw(I2C_TypeDef * m_I2C, short address, unsigned char * data, int data_len)
{
    int i;

    i2c_start();
    i2c_address_write(address | 0x01);
    if (data_len == 1)
        i2c_next_no_ack();
    else
        i2c_next_ack();

    for (i = 0; i < data_len ;i++) {

        i2c_read(data[i]);
        if (i < (data_len - 2)) {
            i2c_next_ack();
        }
        else {
            i2c_next_no_ack();
        }
    }

    i2c_stop();

    return 0;
}
