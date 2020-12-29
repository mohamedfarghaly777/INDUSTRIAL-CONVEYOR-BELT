
#include "LIB/STD_TYPES.h"
#include "LIB/BIT_MATH.h"

#include "DIO_int.h"
#include "DIO_reg.h"

/* Define Array of pointers to hold all registers */
volatile u8* DIO_OutRegisters [4] = {PORTA_Register, PORTB_Register, PORTC_Register, PORTD_Register};
volatile u8* DIO_InRegisters  [4] = {PINA_Register , PINB_Register , PINC_Register , PIND_Register };
volatile u8* DIO_DirRegisters [4] = {DDRA_Register , DDRB_Register , DDRC_Register , DDRD_Register };

/* IO Pins */
void DIO_SetPinValue(u8 u8PortIdCopy , u8 u8PinIdCopy, u8 u8PinValCopy)
{
	/* Make sure that the Port ID and Pin ID are in the valid range */
	if ((u8PortIdCopy <= PORTD) && (u8PinIdCopy <= PIN7))
	{
		if (u8PinValCopy == HIGH)
		{
			SET_BIT(*DIO_OutRegisters[u8PortIdCopy],u8PinIdCopy);
		}

		else if (u8PinValCopy == LOW)
		{
			CLR_BIT(*DIO_OutRegisters[u8PortIdCopy],u8PinIdCopy);
		}

		else
		{
			/* Do nothing, Wrong value to set */
		}
	}

	else
	{
		/* Do nothing, Error in the Pin ID or PORT ID */
	}
}

u8 DIO_GetPinValue(u8 u8PortIdCopy, u8 u8PinIdCopy)
{
	/* Define Local Variable to get the BIT Value */
	u8 u8ResultLocal;
	/* Make sure that the Port ID and Pin ID are in the valid range */
	if ((u8PortIdCopy <= PORTD) && (u8PinIdCopy <= PIN7))
	{
		u8ResultLocal= GET_BIT(*DIO_InRegisters[u8PortIdCopy],u8PinIdCopy);
	}

	else
	{
		/* return 0xff in case of error in the Pin ID or PORT ID */
		u8ResultLocal = 0xFF;
	}

	return u8ResultLocal;
}

void DIO_SetPinDirection (u8 u8PortIdCopy, u8 u8PinIdCopy, u8 u8PinDirCopy)
{
	/* Make sure that the Port ID and Pin ID are in the valid range */
	if ((u8PortIdCopy <= PORTD) && (u8PinIdCopy <= PIN7))
	{
		if ( u8PinDirCopy == OUTPUT )
		{
			SET_BIT(*DIO_DirRegisters[u8PortIdCopy],u8PinIdCopy);
		}

		else if ( u8PinDirCopy == INPUT )
		{
			CLR_BIT(*DIO_DirRegisters[u8PortIdCopy],u8PinIdCopy);
		}

		else
		{
			/* Do nothing, Wrong Direction Required */
		}
	}

	else
	{
		/* Do nothing, Error in the Pin ID or PORT ID */
	}
}


/* IO Ports */
void DIO_SetPortDirection (u8 u8PortId, u8 u8PortDir)
{
	/* Check on the Required PORT Number */
	if (u8PortId <= PORTD)
	{
		*DIO_DirRegisters[u8PortId] = u8PortDir;
	}

	else
	{
		/* Wrong Port ID */
	}
}

void DIO_SetPortValue (u8 u8PortId, u8 u8PortVal)
{
	/* Check on the Required PORT Number */
	if (u8PortId <= PORTD)
	{
		*DIO_OutRegisters[u8PortId] = u8PortVal;
	}

	else
	{
		/* Wrong Port ID */
	}
}
