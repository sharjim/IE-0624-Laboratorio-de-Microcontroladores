#ifndef GYR_SPI_H_INCLUDED
#define GYR_SPI_H_INCLUDED

#define GYR_SPI		SPI1

#define GYR_RNW			(1 << 7) /* Write when zero */
#define GYR_MNS			(1 << 6) /* Multiple reads when 1 */
#define GYR_WHO_AM_I		0x0F
#define GYR_OUT_TEMP		0x26
#define GYR_STATUS_REG		0x27
#define GYR_CTRL_REG1		0x20
#define GYR_CTRL_REG1_PD	(1 << 3)
#define GYR_CTRL_REG1_XEN	(1 << 1)
#define GYR_CTRL_REG1_YEN	(1 << 0)
#define GYR_CTRL_REG1_ZEN	(1 << 2)
#define GYR_CTRL_REG1_BW_SHIFT 	4
#define GYR_CTRL_REG4			0x23
#define GYR_CTRL_REG4_FS_SHIFT	4

#define SENSITIVITY_250DPS 0.00875F
#define SENSITIVITY_500DPS 0.0175F
#define SENSITIVITY_2000DPS 0.07F

#define GYR_OUT_X_L	0x28
#define GYR_OUT_X_H	0x29
#define GYR_OUT_Y_L	0x2A
#define GYR_OUT_Y_H	0x2B
#define GYR_OUT_Z_L	0x2C
#define GYR_OUT_Z_H	0x2D

void gyr_setup(void);
int16_t gyr_readX(void);
int16_t gyr_readY(void);
int16_t gyr_readZ(void);

#endif // GYR_SPI_H_INCLUDED
