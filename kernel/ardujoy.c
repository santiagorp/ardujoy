/*
 *  Copyright (c) 1999-2001 Vojtech Pavlik
 */

/*
 * Logitech WingMan Warrior joystick driver for Linux
 */

/*
 * This program is free warftware; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 *  Should you need to contact me, the author, you can do so either by
 * e-mail - mail your message to <vojtech@ucw.cz>, or by paper mail:
 * Vojtech Pavlik, Simunkova 1594, Prague 8, 182 00 Czech Republic
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Notung");
MODULE_DESCRIPTION("Arduino joystick driver module");

// Parameters can be changed loading the module. Example:
// #insmod ardujoy.ko ttyDev="/dev/ttyUSB2"
static char *ttyDev = "/dev/ttyUSB0";
module_param(ttyDev, charp, S_IRUGO);

static int serial_fd;

static int __init ardujoy_start(void) {
  printk(KERN_INFO "Loading ardujoy!\n");
  printk(KERN_INFO "Device: %s\n", ttyDev);
  
  lin

  return 0;
}

static void __exit ardujoy_end(void) {
  printk(KERN_INFO "Unloaded ardujoy module\n");
}

module_init(ardujoy_start);
module_exit(ardujoy_end);
