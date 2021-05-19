/*
||
|| @file Scheduler.h
|| @version 1.1
|| @author Alexander Brevig
|| @contact alexanderbrevig@gmail.com
||
|| @description
|| | Provide an easy way of scheduling function calls
|| #
||
|| @license
|| | This library is free software; you can redistribute it and/or
|| | modify it under the terms of the GNU Lesser General Public
|| | License as published by the Free Software Foundation; version
|| | 2.1 of the License.
|| |
|| | This library is distributed in the hope that it will be useful,
|| | but WITHOUT ANY WARRANTY; without even the implied warranty of
|| | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
|| | Lesser General Public License for more details.
|| |
|| | You should have received a copy of the GNU Lesser General Public
|| | License along with this library; if not, write to the Free Software
|| | Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
|| #
||
*/

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <WProgram.h>

//provide a typedef for a void function pointer
typedef void (*function)();

typedef struct schedule_action_s {
	function action;
	unsigned long time;
	unsigned int delay;
	//assignment operator
	schedule_action_s& operator=(const schedule_action_s& rhs){
		action = rhs.action;
		time = rhs.time;
		delay = rhs.delay;
		return *this;
	}
	//boolean equality
	bool operator==(const schedule_action_s& rhs) const{
		return (action == rhs.action && time == rhs.time && delay == rhs.delay);
	}
}schedule_action;


class Scheduler {
	public:
		Scheduler();
		void update();
		void schedule(function userAction, unsigned int time);
	private:
		static const byte NUMBER_OF_SCHEDULED_ACTIONS = 10;
		void defragment();
		schedule_action action[NUMBER_OF_SCHEDULED_ACTIONS];
		byte currentSize;
};

#endif

/*
|| @changelog
|| | 1.0 2009-07-21 - Alexander Brevig : Changed Lisence
|| | 1.0 2009-05-09 - Alexander Brevig : Initial Release
|| #
*/