//----------------------------------------------------------------------------
//
// Project      : Call To Power 2
// File type    : C++ source
// Description  : Squad strength object 
//
//----------------------------------------------------------------------------
//
// Disclaimer
//
// THIS FILE IS NOT GENERATED OR SUPPORTED BY ACTIVISION.
//
// This material has been developed at apolyton.net by the Apolyton CtP2
// Source Code Project. Contact the authors at ctp2source@apolyton.net.
//
//----------------------------------------------------------------------------
//
// Compiler flags
//
//----------------------------------------------------------------------------
//
// Modifications from the original Activision code:
//
// - Changed the > operator (used by Goal::IsSatisfied method)
//   Original method only test attack or defense param.
//   New methode take the sum of all strength (attack, defense, ranged,...)
//   by Calvitix and Martin G�hmann.
// - Handled problem with invalid units.
// 
//----------------------------------------------------------------------------

#include "c3.h"
#include "squad_Strength.h"

#include "agent.h"
#include "cellunitlist.h"
#include "World.h"
#include "UnitRecord.h"
#include "debugassert.h"

Squad_Strength::Squad_Strength(const Squad_Strength &squad_strength)
{
	*this = squad_strength;
}

Squad_Strength & Squad_Strength::operator= (const Squad_Strength &squad_strength)
{
	
    m_agent_count = squad_strength.m_agent_count;  

	
    m_attack_str = squad_strength.m_attack_str;

	
    m_defense_str = squad_strength.m_defense_str;

	m_ranged_str = squad_strength.m_ranged_str;

	
	m_value = squad_strength.m_value;

	
	m_transport = squad_strength.m_transport;

	m_defenders = squad_strength.m_defenders;

    m_ranged = squad_strength.m_ranged; 

	m_land_bombard_str = squad_strength.m_land_bombard_str;
	m_water_bombard_str = squad_strength.m_water_bombard_str;
	m_air_bombard_str = squad_strength.m_air_bombard_str;

    return *this;
}

bool Squad_Strength::operator> (const Squad_Strength &squad_strength) const
{
    // Transport squads should always be always bigger:
    if (m_transport > squad_strength.m_transport)
    {
        return true;
    }
    else if (m_transport < squad_strength.m_transport)
    {
	  return false;
    }

    // Equal transport strength: test the battle strength

    // Attack difference
    sint16 const attack_cpr = (m_attack_str - squad_strength.m_attack_str);
    // Defense difference
    sint16 const defense_cpr = (m_defense_str - squad_strength.m_defense_str);
    // ranged difference
    sint16 const ranged_cpr = (m_ranged_str - squad_strength.m_ranged_str);
    // value difference
    sint16 const value_cpr = (m_value - squad_strength.m_value);

    sint16 const battle_cpr = attack_cpr	+ defense_cpr + ranged_cpr + value_cpr;
    if (battle_cpr > 0)
    {
        return true;
    }
    else if (battle_cpr < 0)
    {
        return false;
    }

    // Equal battle strength: test the agent count

    if (m_agent_count > 0 && squad_strength.m_agent_count > 0){

		//If only agent count is a criterion : (for special units for example)
		if(m_attack_str + m_defense_str + m_ranged_str + m_value  == 0
		&& squad_strength.m_attack_str + squad_strength.m_defense_str + squad_strength.m_ranged_str + squad_strength.m_value  == 0
		){
			return (m_agent_count > squad_strength.m_agent_count);
		}
	}
    return false;
}


Squad_Strength & Squad_Strength::operator+=(const Squad_Strength & add_me)
{
	m_agent_count += add_me.m_agent_count;
	m_attack_str += add_me.m_attack_str;
	m_defense_str += add_me.m_defense_str;
	m_ranged_str += add_me.m_ranged_str;
	m_value += add_me.m_value;
	m_transport += add_me.m_transport;
	m_defenders += add_me.m_defenders;
    m_ranged += add_me.m_ranged; 

	m_land_bombard_str += add_me.m_land_bombard_str;
	m_water_bombard_str += add_me.m_water_bombard_str;
	m_air_bombard_str += add_me.m_air_bombard_str;

    return *this;
} 


Squad_Strength & Squad_Strength::operator-=(const Squad_Strength & remove_me)
{
	m_agent_count -= remove_me.m_agent_count;
	m_attack_str -= remove_me.m_attack_str;
	m_defense_str -= remove_me.m_defense_str;
	m_ranged_str -= remove_me.m_ranged_str;
	m_value -= remove_me.m_value;
	m_transport -= remove_me.m_transport;
	m_defenders -= remove_me.m_defenders;
    m_ranged -= remove_me.m_ranged; 
	m_land_bombard_str -= remove_me.m_land_bombard_str;
	m_water_bombard_str -= remove_me.m_water_bombard_str;
	m_air_bombard_str -= remove_me.m_air_bombard_str;

    return *this;
}


sint32 Squad_Strength::Get_Agent_Count() const
{
	return m_agent_count;
} 


void Squad_Strength::Set_Agent_Count(const sint32 & count)
{
	m_agent_count = count;
}


void Squad_Strength::Add_Agent_Strength(const Agent_ptr & agent)
{
	(*this) += agent->Get_Squad_Strength();
}


void Squad_Strength::Remove_Agent_Strength(const Agent_ptr & agent)
{
	(*this) -= agent->Get_Squad_Strength();
}


void Squad_Strength::Set_Pos_Strength(const MapPoint & pos)
{
	CellUnitList *army = g_theWorld->GetArmyPtr(pos);

	
	if (army == NULL)
	{
		*this           = Squad_Strength(1); // why not 0 ??? 
		return;
	}

	m_agent_count = army->Num();

	
	army->ComputeStrength(m_defense_str,	
						  m_attack_str,		
						  m_ranged_str,		
						  m_defenders,		
						  m_ranged,
						  m_land_bombard_str,
						  m_water_bombard_str,
						  m_air_bombard_str);		
   
	
	m_value = 0.0;
	m_transport = 0;
	for (int i = m_agent_count; i > 0; --i)
	{
		Unit const &	unit	= army->Get(i - 1);
		
		if (unit.IsValid())
		{
			m_value		+= unit.GetDBRec()->GetShieldCost();
			m_transport	+= (unit.GetCargoCapacity() - unit.GetNumCarried());
		}
		else
		{
			// Remove the invalid unit
			army->DelIndex(i - 1);
			--m_agent_count;
		}
	}
}


double Squad_Strength::Get_Attack() const
{
	return m_attack_str;
} 


void Squad_Strength::Set_Attack(const double & attack)
{
	m_attack_str = attack;
} 


double Squad_Strength::Get_Defense() const
{
	return m_defense_str;
} 


void Squad_Strength::Set_Defense(const double & defense)
{
	m_defense_str = defense;
} 


double Squad_Strength::Get_Ranged() const
{
	return m_ranged_str;
}


void Squad_Strength::Set_Ranged(const double & ranged)
{
	m_ranged_str = ranged;
}


double Squad_Strength::Get_Value() const
{
	return m_value;
} 


void Squad_Strength::Set_Value(const double & value)
{
	m_value = value;
} 


sint16 Squad_Strength::Get_Transport() const
{
	return m_transport;
}
	

void Squad_Strength::Set_Transport(const sint16 & slots)
{
	m_transport = slots;
}


sint16 Squad_Strength::Get_Defenders() const
{
	return m_defenders;
}


void Squad_Strength::Set_Defenders(const sint16 & units)
{
	m_defenders = units;
}


sint16 Squad_Strength::Get_Ranged_Units() const
{
	return m_ranged;
}


void Squad_Strength::Set_Ranged_Units(const sint16 & units)
{
	m_ranged = units;
}


void Squad_Strength::Set_Force_Matching( const double attack_ratio,
										const double defense_ratio,
										const double ranged_ratio,
										const double bombard_ratio,
										const double value_ratio )
{
	m_attack_str *= attack_ratio;
	m_defense_str *= defense_ratio;
	m_ranged_str *= ranged_ratio;
	m_land_bombard_str *= bombard_ratio;
	m_water_bombard_str *= bombard_ratio;
	m_air_bombard_str *= bombard_ratio;
	m_value *= value_ratio;
}
