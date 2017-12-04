/*
 * emac.h
 *
 *  Created on: 2012-9-13
 *      Author: wangjie
 */

#ifndef EMAC_H_
#define EMAC_H_
#include "c6678.h"

void C6678_ConfigSerdes();
void C6678_Init_SGMII_Force (uint32_t macPortNum);

void C6678_Init_SGMII_Auto (uint32_t macPortNum);

void C6678_Init_SGMII (uint32_t macPortNum);

#endif /* EMAC_H_ */
