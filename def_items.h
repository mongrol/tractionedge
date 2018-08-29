/*
 *  def_items.h
 *  TractionEdge
 *
 *  Created by Steven Hamilton on 7/11/09.
 *  Copyright 2009 scorch.org. All rights reserved.
 *
 */
#ifndef __DEF_ITEMS_H__
#define __DEF_ITEMS_H__

enum itemType_t{
	CORPSE,
    
    ARMOUR_LEATHER,
    ARMOUR_IRON,
	
    PISTOL,
	MUSKET,
    MUSKETOON,

    CUTLASS,
    BILLYCLUB,
    BRASS_FANG,
    WOOD_MANDIBLE
};

enum itemAttackType_t{
    MELEE,
    RANGED,
    NO_ATTACK_TYPE
};

#endif
