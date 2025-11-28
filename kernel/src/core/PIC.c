#include "PIC.h"
#define PIC1             0x20
#define PIC2            0xA0
#define PIC1_COMMAND    PIC1
#define PIC1_DATA       (PIC1+1)
#define PIC2_COMMAND    PIC2
#define PIC2_DATA       (PIC2+2)

#define PIC_EOI         0x20

#define ICW1_ICW4	0x01		/* Indicates that ICW4 will be present */
#define ICW1_SINGLE	0x02		/* Single (cascade) mode */
#define ICW1_INTERVAL4	0x04		/* Call address interval 4 (8) */
#define ICW1_LEVEL	0x08		/* Level triggered (edge) mode */
#define ICW1_INIT	0x10		/* Initialization - required! */

#define ICW4_8086	0x01		/* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO	0x02		/* Auto (normal) EOI */
#define ICW4_BUF_SLAVE	0x08		/* Buffered mode/slave */
#define ICW4_BUF_MASTER	0x0C		/* Buffered mode/master */
#define ICW4_SFNM	0x10		/* Special fully nested (not) */

#define CASCADE_IRQ 2

#define PIC_READ_IRR                0x0a
#define PIC_READ_ISR                0x0b

void PIC_sendEOI(uint8_t irq) {
    if (irq >= 8) {
        port_byte_out(PIC2_COMMAND, PIC_EOI);
    }

    print_string("PIC_sendEOI fired!", 0xFF00FF00);

    port_byte_out(PIC1_COMMAND, PIC_EOI);
}

void PIC_remap(int offset1, int offset2)
{
	port_byte_out(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
	io_wait();
	port_byte_out(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
	io_wait();
	port_byte_out(PIC1_DATA, offset1);
	io_wait();
	port_byte_out(PIC2_DATA, offset2);
	io_wait();
	port_byte_out(PIC1_DATA, 1 << CASCADE_IRQ);
	io_wait();
	port_byte_out(PIC2_DATA, 2);
	io_wait();
	
	port_byte_out(PIC1_DATA, ICW4_8086);
	io_wait();
	port_byte_out(PIC2_DATA, ICW4_8086);
	io_wait();

	port_byte_out(PIC1_DATA, 0);
	port_byte_out(PIC2_DATA, 0);
}

void PIC_disable(void) {
    port_byte_out(PIC1_DATA, 0xff);
    port_byte_out(PIC2_DATA, 0xff);
}

void IRQ_set_mask(uint8_t IRQline) {
    uint16_t port;
    uint8_t value;

    if(IRQline < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        IRQline -= 8;
    }
    value = port_byte_in(port) | (1 << IRQline);
    port_byte_out(port, value);        
}

void IRQ_clear_mask(uint8_t IRQline) {
    uint16_t port;
    uint8_t value;

    if(IRQline < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        IRQline -= 8;
    }
    value = port_byte_in(port) & ~(1 << IRQline);
    port_byte_out(port, value);        
}

static uint16_t __pic_get_irq_reg(int ocw3)
{
    port_byte_out(PIC1_COMMAND, ocw3);
    port_byte_out(PIC2_COMMAND, ocw3);
    return (port_byte_in(PIC2_COMMAND) << 8) | port_byte_in(PIC1_COMMAND);
}

uint16_t pic_get_irr(void)
{
    return __pic_get_irq_reg(PIC_READ_IRR);
}

uint16_t pic_get_isr(void)
{
    return __pic_get_irq_reg(PIC_READ_ISR);
}