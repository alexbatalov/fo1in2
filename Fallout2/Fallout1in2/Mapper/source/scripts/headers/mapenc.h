/*

   Generic Encounters

*/

#pragma once

/************************************************
    Generic Defines
************************************************/

// All Map Vars need to start w/ MVAR_
#define MVAR_Hostile_Total             (0) // Number of monsters created
#define MVAR_CARVN_LEAD                (1) // Used in caravan encounter
#define MVAR_CAVERN_TYPE               (2) // Will special scenery spawn in this location?
#define MVAR_CAVERN_LADDER_ELEV1       (3) // Used to remember the position of the randomly placed ladder
#define MVAR_CAVERN_LADDER_ELEV2       (4) // Ladder pos on elev2

// The Fo2 perk "Cautious Nature" doesn't work in the Fo1 encounter system, so we build our own functionality
// NOTE: We "secretly" tripple the PE bonus from 3 to 12, otherwise the effect is impossible to notice in the Fo1 encounter spawn system.
#define dude_enc_perception            (dude_perception + (9 * dude_perk(PERK_cautious_nature_perk)))

// Worldmap tile names - unused for now, maybe useful once wm parsing in gl_worldmap.ssl is deprecated
/*
#define is_terrain_desert              (get_current_terrain_name == mstr_worldmap(1000))
#define is_terrain_mountains           (get_current_terrain_name == mstr_worldmap(1001))
#define is_terrain_city                (get_current_terrain_name == mstr_worldmap(1002))
#define is_terrain_coast               (get_current_terrain_name == mstr_worldmap(1003))
*/

// Where the player will get radiation on the worldmap
#define is_terrain_radiated_mountains  (get_current_terrain_name == mstr_worldmap(1005))
#define is_terrain_radiated_desert     (get_current_terrain_name == mstr_worldmap(1006))
#define is_radiated_wm_tile            (is_terrain_radiated_mountains or is_terrain_radiated_desert)

#define SetGlowEncRads     if (dude_elevation != 1) then radiation_inc(dude_obj, random(1, 3)); \
                           add_timer_event(self_obj, game_ticks(random(5, 10)), EVENT_GLOW_RADS)

// Cavern Type for random scenery spawn
#define TOXIC_CAVE         (1)
#define GOLD_MINE          (2)
#define HIDEOUT_1          (3)
#define DEAD_BODIES        (4)
#define ROBOT_CLEANER      (5)
#define CENTAUR_HANDLER    (6)

#define set_gold_mine         set_map_var(MVAR_CAVERN_TYPE, GOLD_MINE); debug_cyan("Setting Gold Mine!")
#define set_toxic_cave        set_map_var(MVAR_CAVERN_TYPE, TOXIC_CAVE); debug_cyan("Setting Toxic Cave!")
#define set_hideout_1         set_map_var(MVAR_CAVERN_TYPE, HIDEOUT_1); debug_cyan("Setting Hideout!")
#define set_dead_bodies       set_map_var(MVAR_CAVERN_TYPE, DEAD_BODIES); debug_cyan("Setting Dead Bodies!")
#define set_robot_cleaner     set_map_var(MVAR_CAVERN_TYPE, ROBOT_CLEANER); debug_cyan("Setting Robot Cleaner!")
#define set_centaur_handler   set_map_var(MVAR_CAVERN_TYPE, CENTAUR_HANDLER); debug_cyan("Setting Centaur Handler!")

#define cave_is_gold_mine           (map_var(MVAR_CAVERN_TYPE) == GOLD_MINE)
#define cave_is_toxic               (map_var(MVAR_CAVERN_TYPE) == TOXIC_CAVE)
#define cave_is_hideout_1           (map_var(MVAR_CAVERN_TYPE) == HIDEOUT_1)
#define cave_is_bodies              (map_var(MVAR_CAVERN_TYPE) == DEAD_BODIES)
#define cave_is_robot_cleaner       (map_var(MVAR_CAVERN_TYPE) == ROBOT_CLEANER)
#define cave_is_centaur_handler     (map_var(MVAR_CAVERN_TYPE) == CENTAUR_HANDLER)

// Gecko hunter bits
#define HUNTER_FATHER_DEAD          bit_1
#define HUNTER_SON_DEAD             bit_2
#define HUNTER_WAR_PARTY            bit_3
#define HUNTER_PC_KNOWS_NAMES       bit_4
#define HUNTER_KNOWS_PC_NAME        bit_5

#define set_ghunter_status(x)       set_gvar_bit_on(GVAR_GECKO_HUNTER_STATUS, x)
#define ghunter_status(x)           (gvar_bit(GVAR_GECKO_HUNTER_STATUS, x))

#define ghunters_killed             (gvar_bit(GVAR_GECKO_HUNTER_STATUS, HUNTER_FATHER_DEAD) and gvar_bit(GVAR_GECKO_HUNTER_STATUS, HUNTER_SON_DEAD))
#define ghunters_party_killed       (gvar_bit(GVAR_GECKO_HUNTER_STATUS, HUNTER_WAR_PARTY))
#define ghunters_pc_knows_names     (gvar_bit(GVAR_GECKO_HUNTER_STATUS, HUNTER_PC_KNOWS_NAMES))
#define ghunters_know_pc_name       (gvar_bit(GVAR_GECKO_HUNTER_STATUS, HUNTER_KNOWS_PC_NAME))

// Amount of tribesman hunting the player
#define GHUNTER_REVENGE_COUNT       (5)

#define set_tribesman_count         if (global_var(GVAR_GECKO_HUNTER_WARPARTY) == 0) then \
                                       set_global_var(GVAR_GECKO_HUNTER_WARPARTY, GHUNTER_REVENGE_COUNT)

//==================================================================
// Debug stuff. Override the next encounter table.
#define enc_override \
   if (global_var(GVAR_RANDOM_ENC_OVERRIDE) > 0) then begin                                  \
      Encounter_Num := global_var(GVAR_RANDOM_ENC_OVERRIDE);                                 \
      debug_msg("Overridding encounter with type " + global_var(GVAR_RANDOM_ENC_OVERRIDE));  \
      set_global_var(GVAR_RANDOM_ENC_OVERRIDE, 0);                                           \
   end                                                                                       \
   noop


//==================================================================
#define spawn_dead_critter(x,y,z)      \
   Critter_type := x;                  \
   Critter_script := y;                \
   Critter_direction := random(0, 5);  \
   call Place_critter;                 \
   anim(Critter, 1000, random(0, 5));  \
   kill_critter(Critter, z)

#define enc_group_angle(x)                   \
   if (group_angle == 0) then begin          \
      x := 3;                                \
   end                                       \
   else if (group_angle == 1) then begin     \
      x := 4;                                \
   end                                       \
   else if (group_angle == 2) then begin     \
      x := 5;                                \
   end                                       \
   else if (group_angle == 3) then begin     \
      x := 0;                                \
   end                                       \
   else if (group_angle == 4) then begin     \
      x := 1;                                \
   end                                       \
   else if (group_angle == 5) then begin     \
      x := 2;                                \
   end

// Ranger-Perk encounter re-roll
#define ranger_perk_reroll_enc(a,b,c,d,e) \
   while (Ranger_rerolls > 0) do begin                                                                                                       \
      Ranger_rerolls := (Ranger_rerolls - 1);                                                                                                \
      if ((Encounter_Num == a) or (Encounter_Num == b) or (Encounter_Num == c) or (Encounter_Num == d) or (Encounter_Num == e)) then begin   \
         debug("Ranger Perk: Re-roll encounter! " + Encounter_Num);                                                                          \
         call roll_encounter;                                                                                                                \
      end                                                                                                                                    \
   end \
   noop

// Spawn cavern critters
#define AREABLK(area)                                          \
   count := total_encounter_mobs;                              \
   while (count > 0) do begin                                  \
      call Choose_Pid;                                         \
      call placeCritter(choose_enc_pid, choose_enc_sid, area); \
      count -= 1;                                              \
   end

// Temp variables
variable
   scenery_elevation := 0,
   Range,

   dude_pos,
   dude_rot,
   Critter_spawn_hex,
   Encounter_Num,
   Ranger_rerolls,

   Tot_Critter_A,
   Tot_Critter_B,
   victim,

   group_angle,
   Critter,
   Critter_direction,
   Critter_script := -1,
   Critter_tile,
   Critter_type,
   CritterXpos,
   CritterYpos,

   Skill_roll,

   Inner_ring,
   Item,
   Outer_ring;

procedure Place_critter;

/************************************************
    Generate Scenery for Desert maps
************************************************/
critical procedure Scenes(variable scenery_elevation) begin
   variable LVar2 := 0;
   variable LVar3 := 0;
   variable LVar4 := 0;
   variable Place_Holder;
   variable spawn_hex;

   if (Critter_spawn_hex == 0) then Critter_spawn_hex := dude_tile;

   Tot_Critter_A := random(3, 14);
   LVar4 := -1;
   while(Tot_Critter_A) do begin
      Critter_direction := random(0, 5);
      Range := random(4, 28);
      Place_Holder := Range;
      spawn_hex := tile_num_in_direction(Critter_spawn_hex, Critter_direction, Place_Holder);
      Critter_direction := random(0, 5);
      spawn_hex := tile_num_in_direction(Critter_spawn_hex, Critter_direction, Place_Holder);
      spawn_hex := spawn_hex + (random(0, 2) - 1);
      Critter_direction := random(0, 5);
      spawn_hex := tile_num_in_direction(Critter_spawn_hex, Critter_direction, Place_Holder);
      spawn_hex := spawn_hex + (random(0, 2) - 1);
      Critter_direction := random(0, 5);
      spawn_hex := tile_num_in_direction(Critter_spawn_hex, Critter_direction, Place_Holder);
      LVar2 := random(0, 9);
      while(LVar4 == LVar2) do begin
         LVar2 := random(0, 9);
      end
      if (LVar2 == 0) then begin
         LVar3 := random(1, 6);
         if (LVar3 == 1) then begin
            Item := create_object(PID_BONES_4, spawn_hex, scenery_elevation);
         end
         else if (LVar3 == 2) then begin
            if (random(1, 4) == 1) then begin
               Item := create_object(PID_TREE_2, spawn_hex, scenery_elevation);
            end
            else begin
               Item := create_object(PID_CACTUS_1,spawn_hex, scenery_elevation);
            end
         end
         else if (LVar3 == 3) then begin
            Item := create_object(PID_DIRT_5, spawn_hex, scenery_elevation);
         end
         else if (LVar3 == 4) then begin
            Item := create_object(PID_CACTUS_1, spawn_hex, scenery_elevation);
         end
         else if (LVar3 == 5) then begin
            Item := create_object(PID_CACTUS_2, spawn_hex, scenery_elevation);
         end
         else if (random(1, 3) == 1) then begin
            Item := create_object(PID_TREE_OLD, spawn_hex, scenery_elevation);
         end
         else begin
            Item := create_object(PID_TRASH_2, spawn_hex, scenery_elevation);
         end
      end
      else if (LVar2 == 1) then begin
         LVar3 := random(1, 3);
         if (LVar3 == 1) then begin
            Item := create_object(PID_ROCKS_8, spawn_hex, scenery_elevation);
         end
         else if (LVar3 == 2) then begin
            Item := create_object(PID_ROCKS_7, spawn_hex, scenery_elevation);
         end
         else begin
            Item := create_object(PID_ROCKS_6, spawn_hex, scenery_elevation);
         end
      end
      else if (LVar2 == 2) then begin
         LVar3 := random(1, 5);
         if (LVar3 == 1) then begin
            Item := create_object(PID_BONES_1, spawn_hex, scenery_elevation);
         end
         else if (LVar3 == 2) then begin
            Item := create_object(PID_BONES_2, spawn_hex, scenery_elevation);
         end
         else if (LVar3 == 3) then begin
            Item := create_object(PID_BONES_3, spawn_hex, scenery_elevation);
         end
         else if (LVar3 == 4) then begin
            Item := create_object(PID_BONES_4, spawn_hex, scenery_elevation);
         end
         else begin
            Item := create_object(PID_BONES_5, spawn_hex, scenery_elevation);
         end
      end
      else if (LVar2 == 3) then begin
         Item := create_object(PID_TIRE, spawn_hex, scenery_elevation);
      end
      else if (LVar2 == 4) then begin
         LVar3 := random(1, 4);
         if (LVar3 == 1) then begin
            Item := create_object(PID_ROCKS_3, spawn_hex, scenery_elevation);
         end
         else if (LVar3 == 2) then begin
            Item := create_object(PID_ROCKS_4, spawn_hex, scenery_elevation);
         end
         else if (LVar3 == 3) then begin
            Item := create_object(PID_ROCKS_3, spawn_hex, scenery_elevation);
         end
         else begin
            Item := create_object(PID_ROCKS_4, spawn_hex, scenery_elevation);
         end
      end
      else if (LVar2 == 5) then begin
         LVar3 := random(1, 4);
         if (LVar3 == 1) then begin
            Item := create_object(PID_SCRUB_1, spawn_hex, scenery_elevation);
         end
         else if (LVar3 == 2) then begin
            Item := create_object(PID_SCRUB_2, spawn_hex, scenery_elevation);
         end
         else if (LVar3 == 3) then begin
            Item := create_object(PID_SCRUB_3, spawn_hex, scenery_elevation);
         end
         else begin
            Item := create_object(PID_SCRUB_1, spawn_hex, scenery_elevation);
         end
      end
      else if (LVar2 == 6) then begin
         LVar3 := random(1, 3);
         if (LVar3 == 1) then begin
            //Item := create_object(PID_JUNK_TIRE, spawn_hex, scenery_elevation);
            Create_Junk_Tire(spawn_hex, scenery_elevation)
         end
         else if (LVar3 == 2) then begin
            //Item := create_object(PID_JUNK_DIRT_2, spawn_hex, scenery_elevation);
            Create_Junk_Dirt_2(spawn_hex, scenery_elevation)
         end
         else begin
            //Item := create_object(PID_JUNK_DIRT_1, spawn_hex, scenery_elevation);
            Create_Junk_Dirt_1(spawn_hex, scenery_elevation)
         end
      end
      else if (LVar2 == 7) then begin
         LVar3 := random(1, 4);
         if (LVar3 == 1) then begin
            Item := create_object(PID_DIRT_5, spawn_hex, scenery_elevation);
         end
         else if (LVar3 == 2) then begin
            Item := create_object(PID_DIRT_3, spawn_hex, scenery_elevation);
         end
         else if (LVar3 == 3) then begin
            Item := create_object(PID_DIRT_6, spawn_hex, scenery_elevation);
         end
         else begin
            Item := create_object(PID_DIRT_4, spawn_hex, scenery_elevation);
         end
      end
      else if (LVar2 == 8) then begin
         LVar3 := random(1, 3);
         if (LVar3 == 1) then begin
            Item := create_object(PID_ROCKS_8, spawn_hex, scenery_elevation);
         end
         else if (LVar3 == 2) then begin
            Item := create_object(PID_ROCKS_7, spawn_hex, scenery_elevation);
         end
         else begin
            Item := create_object(PID_ROCKS_6, spawn_hex, scenery_elevation);
         end
      end
      else begin
         LVar3 := random(1, 6);
         if (LVar3 == 1) then begin
            Item := create_object(PID_BONES_4, spawn_hex, scenery_elevation);
         end
         else if (LVar3 == 2) then begin
            if (random(1, 3) == 1) then begin
               Item := create_object(PID_TREE_2, spawn_hex, scenery_elevation);
            end
            else begin
               Item := create_object(PID_CACTUS_1, spawn_hex, scenery_elevation);
            end
         end
         else if (LVar3 == 3) then begin
            Item := create_object(PID_DIRT_5, spawn_hex, scenery_elevation);
         end
         else if (LVar3 == 4) then begin
            Item := create_object(PID_CACTUS_1, spawn_hex, scenery_elevation);
         end
         else if (LVar3 == 5) then begin
            Item := create_object(PID_CACTUS_2, spawn_hex, scenery_elevation);
         end
         else if (random(1, 3) == 1) then begin
            Item := create_object(PID_TREE_OLD, spawn_hex, scenery_elevation);
         end
         else begin
            Item := create_object(random(0, 2) + 33554710, spawn_hex, scenery_elevation);
         end
      end
      LVar4 := LVar2;
      //debug("try to move item " + Item + " which is " + obj_pid(Item));
      // This can cause empty encounters
      // Don't ask me why, I have no idea. Item is always valid in my tests, and the spawn_hex is ok too.
      // If we just skip this step and directly create the objects on their final position, then this
      // can be circumvented. Not sure yet if this is good or bad, but for now it seems to be a workaround.
      //critter_attempt_placement(Item, spawn_hex, scenery_elevation);
      //debug("finished moving item!");
      Tot_Critter_A := Tot_Critter_A - 1;
   end
   //debug("Tot_Critter_A is completed!");
end

/************************************************
    Mysterious Stranger
************************************************/
procedure Add_Mysterious_Stranger begin
   variable Critter;
   variable Critter_Tile;
   variable Die_Roll;
   variable Stranger_Chance;
   variable val;

   Die_Roll:=random(0,99);
   Stranger_Chance:=30 + (2 * dude_luck);

   if (map_first_run and not(get_stranger_flag(STRANGER_DEAD)) and (Die_Roll < Stranger_Chance) and has_trait(TRAIT_PERK,dude_obj,PERK_mysterious_stranger)) then begin
       if not(get_stranger_flag(STRANGER_GENDER)) then begin
           set_stranger_flag(STRANGER_GENDER);
           if (random(0,99) < 50) then begin
               set_stranger_flag(STRANGER_FEMALE);
           end
       end

       if get_stranger_flag(STRANGER_FEMALE) then
           Critter:=create_object_sid(PID_MYSTERIOUS_STRANGER_FEMALE,0,0,SCRIPT_MYSTSTRN);
       else
           Critter:=create_object_sid(PID_MYSTERIOUS_STRANGER_MALE,0,0,SCRIPT_MYSTSTRN);

       Critter_Tile:=tile_num_in_direction(tile_num(dude_obj),random(0,5),random(5,10));
       critter_attempt_placement(Critter,Critter_Tile,elevation(dude_obj));
       val := critter_add_trait(Critter,TRAIT_OBJECT,OBJECT_TEAM_NUM,TEAM_PLAYER);
       item_caps_adjust(Critter,random(30,60));
       anim(Critter,ANIMATE_ROTATION,dude_cur_rot);
   end
end

/************************************************
    Avellone, the Bounty Hunter + his crew
************************************************/
procedure hunters begin
   display_msg(message_str(SCRIPT_RNDDESRT, 402));

   Inner_ring := 8;
   Outer_ring := 5;
   group_angle := random(0, 5);
   Critter_direction := group_angle + random(0, 3 * 2) - 3;
   while(Critter_direction < 0) do begin
      Critter_direction := Critter_direction + 6;
   end
   if (Critter_direction > 5) then begin
      Critter_direction := Critter_direction % 6;
   end

   // Avellone
   Critter_type := PID_AVELLONE;
   Critter_script := SCRIPT_HUNTER;
   call Place_critter;
   obj_rotate(Critter, rotation_to_tile(tile_num(Critter), dude_tile));

   Item := create_object(PID_ASSAULT_RIFLE, 0, 0);
   add_obj_to_inven(Critter, Item);
   Item := create_object(PID_5MM_AP, 0, 0);
   add_mult_objs_to_inven(Critter, Item, 4 * (dude_perk(PERK_scrounger) + 1));

   Item := create_object(PID_COMBAT_ARMOR, 0, 0);
   add_obj_to_inven(Critter, Item);
   wield_obj_critter(Critter, Item);

   item_caps_adjust(Critter, fortune_finder(random(5, 30)));
   Item := create_object(PID_SUPER_STIMPAK, 0, 0);
   add_mult_objs_to_inven(Critter, Item, 2);

   Critter_direction := group_angle + random(0, 3 * 2) - 3;
   while(Critter_direction < 0) do begin
      Critter_direction := Critter_direction + 6;
   end
   if (Critter_direction > 5) then begin
      Critter_direction := Critter_direction % 6;
   end

   // Thug #1
   Critter_type := PID_THUG_FEMALE_YELLOW;
   Critter_script := SCRIPT_THUG;
   call Place_critter;
   obj_rotate(Critter, rotation_to_tile(tile_num(Critter), dude_tile));

   if (random(0,1) == 1) then begin
      Item := create_object(PID_SPEAR, 0, 0);
      add_obj_to_inven(Critter, Item);
   end
   else begin
      Item := create_object(PID_RIPPER, 0, 0);
      add_obj_to_inven(Critter, Item);
   end
   wield_obj_critter(Critter, Item);

   Item := create_object(PID_STIMPAK, 0, 0);
   add_mult_objs_to_inven(Critter, Item, 3);

   Item := create_object(PID_METAL_ARMOR, 0, 0);
   add_obj_to_inven(Critter, Item);
   wield_obj_critter(Critter, Item);

   if (random(0, 2) == 0) then begin
      item_caps_adjust(Critter, fortune_finder(random(5, 30)));
   end

   Critter_direction := group_angle + random(0, 3 * 2) - 3;
   while(Critter_direction < 0) do begin
      Critter_direction := Critter_direction + 6;
   end
   if (Critter_direction > 5) then begin
      Critter_direction := Critter_direction % 6;
   end

   // Thug #2
   Critter_type := PID_THUG_MALE_METAL;
   Critter_script := SCRIPT_THUG;
   call Place_critter;
   obj_rotate(Critter, rotation_to_tile(tile_num(Critter), dude_tile));

   Item := create_object(PID_SNIPER_RIFLE, 0, 0);
   add_obj_to_inven(Critter, Item);
   Item := create_object(PID_223_FMJ, 0, 0);
   add_mult_objs_to_inven(Critter, Item, 3 * (dude_perk(PERK_scrounger) + 1));

   Item := create_object(PID_METAL_ARMOR, 0, 0);
   add_obj_to_inven(Critter, Item);
   wield_obj_critter(Critter, Item);

   Item := create_object(PID_EXPLOSIVE_ROCKET, 0, 0);
   add_mult_objs_to_inven(Critter, Item, 2 * (dude_perk(PERK_scrounger) + 1));
   if (random(0, 2) == 0) then begin
      item_caps_adjust(Critter, fortune_finder(random(5, 40)));
   end

   Critter_direction := group_angle + random(0, 3 * 2) - 3;
   while(Critter_direction < 0) do begin
      Critter_direction := Critter_direction + 6;
   end
   if (Critter_direction > 5) then begin
      Critter_direction := Critter_direction % 6;
   end

   // Thug #3
   Critter_type := PID_THUG_BLACK_METAL;
   Critter_script := SCRIPT_THUG;
   call Place_critter;
   obj_rotate(Critter, rotation_to_tile(tile_num(Critter), dude_tile));

   Item := create_object(PID_DESERT_EAGLE, 0, 0);
   add_obj_to_inven(Critter, Item);
   Item := create_object(PID_44_MAGNUM_JHP, 0, 0);
   add_mult_objs_to_inven(Critter, Item, 2 * (dude_perk(PERK_scrounger) + 1));

   Item := create_object(PID_METAL_ARMOR, 0, 0);
   add_obj_to_inven(Critter, Item);
   wield_obj_critter(Critter, Item);

   if (random(0, 2) == 0) then begin
      item_caps_adjust(Critter, fortune_finder(random(5, 30)));
   end

   call Add_Mysterious_Stranger;
end

/************************************************
    Dehydration encounters
************************************************/
variable TimeHours := 0;
variable hpDamage := 0;
variable has_water := 0;

#define mstr_item_supply      (message_str(SCRIPT_RNDDESRT, 1250) + obj_name(Item) + message_str(SCRIPT_RNDDESRT, 1251))

#define set_dehydration(x)    Item := create_object(x, 0, 0);              \
                              add_obj_to_inven(dude_obj, Item);            \
                              set_global_var(GVAR_OBJ_DUDE_USE_DRUG, Item)

#define dehydration_knockdown if (hpDamage >= 18) then begin                  \
                                 if (hpDamage >= 24) then begin               \
                                    set_dehydration(PID_DEHYDRATION_STRONG);  \
                                 end else begin                               \
                                    set_dehydration(PID_DEHYDRATION_WEAK);    \
                                 end                                          \
                                 dude_knockdown_nosfx;                        \
                              end                                             \
                              noop

procedure check_water_item begin
   has_water := 0;
   if (party_has_item(PID_NUKA_COLA) or
      party_has_item(PID_WATER_FLASK) or
      party_has_item(PID_BEER) or
      party_has_item(PID_BOOZE) or
      party_has_item(PID_ROENTGEN_RUM) or
      party_has_item(PID_GAMMA_GULP_BEER)) then has_water := 1;
end

procedure drink_water begin
   if (party_has_item(PID_NUKA_COLA)) then
      Item := PID_NUKA_COLA;
   else if (party_has_item(PID_WATER_FLASK)) then
      Item := PID_WATER_FLASK;
   else if (party_has_item(PID_BEER)) then
      Item := PID_BEER;
   else if (party_has_item(PID_BOOZE)) then
      Item := PID_BOOZE;
   else if (party_has_item(PID_ROENTGEN_RUM)) then
      Item := PID_ROENTGEN_RUM;
   else if (party_has_item(PID_GAMMA_GULP_BEER)) then
      Item := PID_GAMMA_GULP_BEER;

   if (Item != 0) then begin
      party_remove_item(Item);
      Item := create_object(Item, 0, 0);
      if (obj_pid(Item) == PID_WATER_FLASK) then
         display_msg(message_str(SCRIPT_RNDDESRT, 125));
      else
         display_msg(mstr_item_supply);
   end
   else
      debug("ERROR! Can't find item for dehydration encounter event!");
end

// Necropolis, Junktown, Brotherhood of Steel, North Table, South Table, Shady Sands, Vats Table
procedure dehydration_a begin
   TimeHours := random(1, 6);

   call check_water_item;

   if (has_water) then begin
      debug("dehydration enc: drink water!");
      call drink_water;
   end
   else begin
      Skill_roll := roll_vs_skill(dude_obj, SKILL_OUTDOORSMAN, 20 * dude_perk(PERK_survivalist));
      if (is_success(Skill_roll)) then begin
         if (TimeHours == 1) then
            display_msg(message_str(SCRIPT_RNDDESRT, 109));
         else
            display_msg(message_str(SCRIPT_RNDDESRT, 110) + TimeHours + message_str(SCRIPT_RNDDESRT, 111));
      end
      else begin
         if (is_critical(Skill_roll)) then begin
            hpDamage := random(15, 24 + TimeHours); // Fo1: 2 to 4
            dehydration_knockdown;
            if (hpDamage >= dude_cur_hp) then hpDamage := dude_cur_hp - 1;
            if (TimeHours == 1) then
               display_msg(message_str(SCRIPT_RNDDESRT, 112) + hpDamage + message_str(SCRIPT_RNDDESRT, 113));
            else
               display_msg(message_str(SCRIPT_RNDDESRT, 114) + TimeHours + message_str(SCRIPT_RNDDESRT, 115) + hpDamage + message_str(SCRIPT_RNDDESRT, 116));
         end
         else begin
            hpDamage := random(6, 12 + TimeHours); // Fo1: 1 to 2
            if (hpDamage >= dude_cur_hp) then hpDamage := dude_cur_hp - 1;
            if (TimeHours == 1) then begin
               if (hpDamage == 1) then
                  display_msg(message_str(SCRIPT_RNDDESRT, 117));
               else
                  display_msg(message_str(SCRIPT_RNDDESRT, 118) + hpDamage + message_str(SCRIPT_RNDDESRT, 119));
            end
            else begin
               if (hpDamage == 1) then
                  display_msg(message_str(SCRIPT_RNDDESRT, 120) + TimeHours + message_str(SCRIPT_RNDDESRT, 121));
               else
                  display_msg(message_str(SCRIPT_RNDDESRT, 122) + TimeHours + message_str(SCRIPT_RNDDESRT, 123) + hpDamage + message_str(SCRIPT_RNDDESRT, 124));
            end
         end
         critter_heal(dude_obj, -hpDamage); // This will not show another message log entry
      end
      TimeHours := TimeHours * ONE_GAME_HOUR;
      game_time_advance(game_ticks(TimeHours));
   end
end

// Hub, Death(claw) table
procedure dehydration_b begin
   TimeHours := random(1, 6) + 2;

   call check_water_item;

   if (has_water) then begin
      call drink_water;
   end
   else begin
      Skill_roll := roll_vs_skill(dude_obj, SKILL_OUTDOORSMAN, 20 * dude_perk(PERK_survivalist));
      if (is_success(Skill_roll)) then begin
         display_msg(message_str(SCRIPT_RNDDESRT, 110) + TimeHours + message_str(SCRIPT_RNDDESRT, 111));
      end
      else begin
         if (is_critical(Skill_roll)) then begin
            hpDamage := random(15, 24 + TimeHours); // Fo1: 2 to 4
         end else begin
            hpDamage := random(6, 12 + TimeHours); // Fo1: 2 to 3
         end
         dehydration_knockdown;
         if (hpDamage >= dude_cur_hp) then hpDamage := dude_cur_hp - 1;
         if (hpDamage == 1) then
            display_msg(message_str(SCRIPT_RNDDESRT, 114) + TimeHours + message_str(SCRIPT_RNDDESRT, 115) + hpDamage + message_str(SCRIPT_RNDDESRT, 1160));
         else
            display_msg(message_str(SCRIPT_RNDDESRT, 114) + TimeHours + message_str(SCRIPT_RNDDESRT, 115) + hpDamage + message_str(SCRIPT_RNDDESRT, 116));
         critter_heal(dude_obj, -hpDamage); // This will not show another message log entry
      end
      TimeHours := TimeHours * ONE_GAME_HOUR;
      game_time_advance(game_ticks(TimeHours));
   end
end

// Mountain encounter
procedure breaking_legs(variable hp_dmg) begin
   if (party_has_item(PID_ROPE)) then begin
      party_remove_item(PID_ROPE);
      display_msg(message_str(SCRIPT_RNDMTN, 204));
   end
   else begin
      if (random(0, 1) == 0) then begin
         critter_injure(dude_obj, DAM_CRIP_LEG_LEFT);
         display_msg(message_str(SCRIPT_RNDMTN, 138) + hp_dmg + message_str(SCRIPT_RNDMTN, 139));
      end
      else begin
         critter_injure(dude_obj, DAM_CRIP_LEG_RIGHT);
         display_msg(message_str(SCRIPT_RNDMTN, 138) + hp_dmg + message_str(SCRIPT_RNDMTN, 140));
      end
   end
end

procedure falling_from_cliff begin
   variable main_roll := 0;
   variable hp_dmg := 0;
   variable hp_cur := 0;

   display_msg(message_str(SCRIPT_RNDMTN, 114));
   main_roll := roll_vs_skill(dude_obj, SKILL_OUTDOORSMAN, ((20 * dude_perk(PERK_survivalist)) - (5 * dude_perception)));
   if (not(is_success(main_roll))) then begin
      if ((random(1, 100) >= 95) or is_critical(main_roll)) then begin
         display_msg(message_str(SCRIPT_RNDMTN, 118));
         hp_dmg := (random(7, 20) + 3);
         hp_cur := dude_cur_hp;
         if (hp_dmg >= hp_cur) then begin
            hp_dmg := (hp_cur - 1);
         end

         reg_anim_clear(dude_obj);
         reg_anim_begin();
            reg_anim_animate(dude_obj, ANIM_fall_back_sf, -1);
            reg_anim_animate(dude_obj, ANIM_back_to_standing, -1);
         reg_anim_end();

         critter_heal(dude_obj, -hp_dmg);
         if (party_has_item(PID_ROPE)) then begin
            party_remove_item(PID_ROPE);
            display_msg(message_str(SCRIPT_RNDMTN, 204));
         end
         else begin
            critter_injure(dude_obj, DAM_CRIP_LEG_LEFT);
            critter_injure(dude_obj, DAM_CRIP_LEG_RIGHT);
            critter_injure(dude_obj, DAM_CRIP_ARM_LEFT);
            critter_injure(dude_obj, DAM_CRIP_ARM_RIGHT);
            display_msg(message_str(SCRIPT_RNDMTN, 147));
         end
      end
      else begin
         display_msg(message_str(SCRIPT_RNDMTN, 117));
         hp_dmg := random(4, 16);
         hp_cur := dude_cur_hp;
         if (hp_dmg >= hp_cur) then begin
            hp_dmg := (hp_cur - 1);
         end

         reg_anim_clear(dude_obj);
         reg_anim_begin();
            reg_anim_animate(dude_obj, ANIM_fall_back_sf, -1);
            reg_anim_animate(dude_obj, ANIM_back_to_standing, -1);
         reg_anim_end();

         critter_heal(dude_obj, -hp_dmg);
         if (random(0,3) == 1) then begin
            display_msg(message_str(SCRIPT_RNDMTN, 138) + hp_dmg + message_str(SCRIPT_RNDMTN, 141));
         end
         else begin
            call breaking_legs(hp_dmg);
         end
      end
   end
   else begin
      display_msg(message_str(SCRIPT_RNDMTN, 115));
      if (is_critical(main_roll)) then begin
         display_msg(message_str(SCRIPT_RNDMTN, 116));
         critter_heal(dude_obj, random(5, 15));
      end
   end
end

