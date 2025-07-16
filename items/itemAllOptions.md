# Item Format Reference

---

## Property Options & Explanations

### type
- "attack": Direct damage or offensive move
- "effect": Buff, debuff, or utility

### effectCategory (for type "effect")
- "buff": Increases stats or grants positive effects
- "debuff": Reduces enemy stats or inflicts negative effects
- "block": Defensive effect

### amountType
- "flat": Fixed value
- "percent": Percentage of stat or damage

### targetType
- "single": One target
- "self": The user
- "allEnemies": All enemies
- "allAllies": All allies
- "randomEnemy": Random enemy
- "randomAlly": Random ally

### attackPassive / enhancedEffect
- Array of effect objects
- effectType: "lifesteal", "heal", "damage", "block", "stun", etc.
- target: "self", "enemy", "allies", etc.
- amount: Numeric value or array
- type: "flat", "percent"
- description: Explanation

### statusEffects
- Array of status effect objects
- type: "bleed", "stun", "freeze", "poison", etc.
- chance: Percent chance
- duration: Number of turns

### passive
- Array of passive effect objects
- effectType: See above
- target: "self", "enemy", etc.
- trigger: "startOfTurn", "onHit", etc.
- amount: Numeric value
- type: "flat", "percent"
- description: Explanation

### gemSlots
- Integer: Number of gem slots

---

# How to Use
- Copy the template above to create a new weapon.
- Fill in each property using the options and explanations.
- Use arrays for any property that can have multiple effects.
- Add comments for clarity if needed.

---

## Example Weapon
```jsonc
{
  "name": "Example Blade", // Weapon name
  "moves": [ // Array of move objects
    {
      "name": "Power Slash", // Name of the move
      "type": "attack", // Move type: "attack" for offensive, "effect" for utility/buff/debuff
      "minEnergy": 2, // Minimum energy required to use the move
      "maxEnergy": 6, // Maximum energy that can be spent on the move
      "amount": [10, 20, 30], // Damage or effect amount based on energy spent: first = minEnergy, second = mid value, third = maxEnergy
      "amountType": "flat", // "flat" for fixed value, "percent" for percentage
      "numAttacks": 1, // Number of attacks performed
      "numTargets": 1, // Number of targets affected
      "targetType": "single", // Target type: "single", "self", "allEnemies", etc.
      "enhancedThreshold": 6, // Energy required to trigger enhanced effect
      "cooldown": 0, // Turns required before move can be used again
      "description": "Deals damage and inflicts bleed.", // Description of the move
      "attackPassive": [ // Array of passive effects applied during attack
        {
          "effectType": "lifesteal", // Type of effect: "lifesteal" heals user for damage dealt
          "target": "self", // Who receives the effect
          "amount": [5, 10, 15], // Amount healed based on energy spent: first = minEnergy, second = mid value, third = maxEnergy
          "type": "flat", // "flat" for fixed value, "percent" for percentage
          "description": "Heals for a portion of damage dealt." // Description of passive effect
        }
      ],
      "statusEffects": [ // Array of status effects applied
        {
          "type": "bleed", // Status effect type: "bleed", "stun", "freeze", etc.
          "chance": 20, // Chance to apply effect (percent)
          "duration": 2 // Duration of effect in turns
        }
      ],
      "enhancedEffect": [ // Array of effects triggered when move is enhanced
        {
          "effectType": "stun", // Type of enhanced effect
          "target": "enemy", // Who receives the effect
          "type": "percent", // "flat" or "percent"
          "amount": 15, // Amount or chance
          "description": "Chance to stun when enhanced." // Description of enhanced effect
        }
      ]
    },
    {
      "name": "Guard Surge", // Name of the move
      "type": "effect", // Move type: "effect" for utility/buff/debuff
      "effectCategory": "buff", // Category of effect: "buff", "debuff", "block"
      "minEnergy": 3, // Minimum energy required
      "maxEnergy": 8, // Maximum energy that can be spent
      "amount": [10, 20, 30], // Effect amount based on energy spent: first = minEnergy, second = mid value, third = maxEnergy
      "amountType": "percent", // "flat" or "percent"
      "numTargets": 1, // Number of targets affected
      "targetType": "self", // Who receives the effect
      "duration": 2, // Duration of effect in turns
      "enhancedThreshold": 8, // Energy required to trigger enhanced effect
      "cooldown": 0, // Turns required before move can be used again
      "description": "Buffs defense for two turns.", // Description of the move
      "attackPassive": [], // Array of passive effects (empty if none)
      "statusEffects": [], // Array of status effects (empty if none)
      "enhancedEffect": [ // Array of effects triggered when move is enhanced
        {
          "effectType": "block", // Type of enhanced effect
          "target": "self", // Who receives the effect
          "type": "percent", // "flat" or "percent"
          "amount": 20, // Amount or chance
          "description": "Blocks additional damage when enhanced." // Description of enhanced effect
        }
      ]
    }
  ],
  "passive": [ // Array of passive effects always active
    {
      "effectType": "energyRestore", // Type of passive effect
      "target": "self", // Who receives the effect
      "trigger": "startOfTurn", // When the effect triggers
      "amount": 2, // Amount restored
      "type": "flat", // "flat" or "percent"
      "description": "Regenerates 2 energy per turn." // Description of passive effect
    }
  ],
  "gemSlots": 2 // Number of gem slots available
}
```

---

# Shield Example
```jsonc
{
  "name": "Guardian Shield", // Shield name
  "moves": [ // Array of move objects
    {
      "name": "Energy Block", // Name of the move
      "type": "effect", // Move type: "effect" for utility/buff/debuff
      "category": "block", // Category of effect: "block" for defensive
      "minEnergy": 3, // Minimum energy required to use the move
      "maxEnergy": 8, // Maximum energy that can be spent on the move
      "amount": [20, 40, 60], // Block percentage based on energy spent: first = minEnergy, second = mid value, third = maxEnergy
      "amountType": "percent", // "flat" for fixed value, "percent" for percentage
      "numAttacks": 1, // Number of blocks performed (usually 1)
      "numTargets": 1, // Number of targets affected (usually self)
      "targetType": "self", // Who receives the effect
      "duration": 1, // Duration of block effect in turns
      "enhancedThreshold": 8, // Energy required to trigger enhanced effect
      "cooldown": 0, // Turns required before move can be used again
      "description": "Blocks incoming damage for one turn. More energy = more block.", // Description of the move
      "attackPassive": [], // Array of passive effects applied during move (empty if none)
      "statusEffects": [], // Array of status effects applied (empty if none)
      "enhancedEffect": [] // Array of effects triggered when move is enhanced (empty if none)
    }
  ],
  "passive": [ // Array of passive effects always active
    {
      "effectType": "damageBlock", // Type of passive effect
      "target": "self", // Who receives the effect
      "trigger": "always", // When the effect triggers
      "amount": 10, // Amount blocked (percent)
      "type": "percent", // "flat" or "percent"
      "description": "Blocks 10% of all incoming damage." // Description of passive effect
    }
  ],
  "gemSlots": 2 // Number of gem slots available
}
```

---

# Gem Examples
```jsonc
{
  "name": "Life Gem", // Gem name
  "effects": { // Effects by equipment type
    "weapon": { // Effects when socketed in a weapon
      "base": { // Base effect
        "effectType": "heal", // Type of effect: "heal" restores HP
        "target": "self", // Who receives the effect
        "percentOfDamage": 20, // Percent of damage dealt converted to healing (single value, not array)
        "description": "Heals 20% of damage dealt as HP." // Description of effect
      },
      "overcharge": { // Overcharge effect (when extra energy is spent)
        "effectType": "heal", // Type of effect
        "target": "allAllies", // Who receives the effect
        "amount": 15, // Amount healed
        "plusEnergy": 3, // Extra energy required
        "description": "Triggers 'Vital Surge': heals all allies for 15 HP." // Description
      }
    },
    "helmet": { // Effects when socketed in a helmet
      "effectType": "energyRestore", // Type of effect
      "target": "self", // Who receives the effect
      "amount": 2, // Amount restored
      "description": "Restores 2 energy per turn." // Description
    },
    "body": { // Effects when socketed in body armor
      "effectType": "heal", // Type of effect
      "target": "self", // Who receives the effect
      "amount": 7, // Amount healed per turn
      "description": "Heals 7 HP per turn." // Description
    },
    "shield": { // Effects when socketed in a shield
      "base": {
        "effectType": "heal", // Type of effect
        "target": "self", // Who receives the effect
        "amount": 10, // Amount healed when blocking
        "description": "Heals 10 HP when blocking." // Description
      },
      "overcharge": {
        "effectType": "block", // Type of effect
        "target": "self", // Who receives the effect
        "blockAll": true, // Whether all damage is blocked
        "heal": 20, // Amount healed
        "plusEnergy": 3, // Extra energy required
        "description": "Triggers 'Barrier Burst': blocks all damage and heals for 20 HP." // Description
      }
    }
  }
}

{
  "name": "Thunder Gem", // Gem name
  "effects": { // Effects by equipment type
    "weapon": { // Effects when socketed in a weapon
      "base": {
        "effectType": "damage", // Type of effect: "damage" adds elemental damage
        "target": "enemy", // Who receives the effect
        "element": "lightning", // Element type
        "amount": "moderate", // Amount of damage
        "description": "Adds lightning damage to attacks." // Description
      },
      "overcharge": {
        "effectType": "damage", // Type of effect
        "target": "allEnemies", // Who receives the effect
        "element": "lightning", // Element type
        "amount": "high", // Amount of damage
        "plusEnergy": 4, // Extra energy required
        "description": "Triggers 'Thunderstorm': deals AoE lightning damage to all enemies." // Description
      }
    },
    "helmet": { // Effects when socketed in a helmet
      "effectType": "reduceCost", // Type of effect
      "target": "self", // Who receives the effect
      "amount": 1, // Amount reduced
      "description": "Reduces energy cost of moves by 1." // Description
    },
    "body": { // Effects when socketed in body armor
      "effectType": "resist", // Type of effect
      "target": "self", // Who receives the effect
      "element": "lightning", // Element type
      "amount": 20, // Amount resisted
      "description": "Reduces lightning damage taken by 20%." // Description
    },
    "shield": { // Effects when socketed in a shield
      "base": {
        "effectType": "reflect", // Type of effect
        "target": "attacker", // Who receives the effect
        "element": "lightning", // Element type
        "amount": 15, // Amount reflected
        "description": "Reflects 15% lightning damage when blocking." // Description
      },
      "overcharge": {
        "effectType": "stun", // Type of effect
        "target": "allAttackers", // Who receives the effect
        "duration": 1, // Duration of stun
        "plusEnergy": 4, // Extra energy required
        "description": "Triggers 'Shockwave': stuns all attackers for 1 turn." // Description
      }
    }
  }
}
```

---

# Armor Examples
```jsonc
{
  "helmet": [ // Array of helmet armor items
    {
      "name": "Crystal Helm", // Helmet name
      "defense": 12, // Defense value provided by helmet
      "passive": [ // Array of passive effects
        {
          "effectType": "magicResist", // Type of passive effect: "magicResist" reduces magic damage
          "target": "self", // Who receives the effect
          "type": "percent", // "flat" or "percent"
          "amount": 10, // Amount resisted (percent, single value)
          "description": "Reduces magic damage taken by 10%." // Description of passive effect
        }
      ],
      "gemSlots": 1 // Number of gem slots available
    }
  ],
  "body": [ // Array of body armor items
    {
      "name": "Vampire Plate", // Body armor name
      "defense": 20, // Defense value provided by body armor
      "passive": [ // Array of passive effects
        {
          "effectType": "healOnHit", // Type of passive effect: "healOnHit" heals user when hit
          "target": "self", // Who receives the effect
          "type": "flat", // "flat" or "percent"
          "amount": 10, // Amount healed (single value)
          "description": "Heals 10 HP on hit." // Description of passive effect
        }
      ],
      "gemSlots": 2 // Number of gem slots available
    }
  ],
  "leggings": [ // Array of leggings armor items
    {
      "name": "Hardened Leggings", // Leggings name
      "defense": 14, // Defense value provided by leggings
      "passive": [ // Array of passive effects
        {
          "effectType": "resist", // Type of passive effect: "resist" reduces damage
          "target": "self", // Who receives the effect
          "type": "percent", // "flat" or "percent"
          "amount": 20, // Amount resisted (percent, single value)
          "description": "Reduces damage taken by 20%." // Description of passive effect
        }
      ],
      "gemSlots": 0 // Number of gem slots available
    }
  ],
  "gloves": [ // Array of gloves armor items
    {
      "name": "Guardian Gauntlets", // Gloves name
      "defense": 8, // Defense value provided by gloves
      "passive": [ // Array of passive effects
        {
          "effectType": "damageBlock", // Type of passive effect: "damageBlock" blocks damage
          "target": "self", // Who receives the effect
          "type": "flat", // "flat" or "percent"
          "amount": 10, // Amount blocked (single value)
          "description": "Blocks 10 points of incoming damage." // Description of passive effect
        }
      ],
      "gemSlots": 0 // Number of gem slots available
    }
  ],
  "boots": [ // Array of boots armor items
    {
      "name": "Thunder Boots", // Boots name
      "defense": 8, // Defense value provided by boots
      "passive": [ // Array of passive effects
        {
          "effectType": "speedBoost", // Type of passive effect: "speedBoost" increases speed
          "target": "self", // Who receives the effect
          "type": "flat", // "flat" or "percent"
          "amount": 2, // Amount of speed boost (single value)
          "description": "+2 speed." // Description of passive effect
        }
      ],
      "gemSlots": 0 // Number of gem slots available
    }
  ]
}
```

---

# Passive Effect Structure
- Use arrays of effect objects for passive effects.
- Each effect object should include: effectType, target, type, amount, description.

# Status Effect Structure
- Use arrays of status effect objects for statusEffects.
- Each status effect object should include: type, chance, duration.

---
