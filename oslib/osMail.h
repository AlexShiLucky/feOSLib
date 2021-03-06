/* FastEmbedded Microcontroller Library
 * Copyright (c) 2018-2028 Gene Kong
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#ifndef __FE_FASTEMBEDDED_OS_MAIL_H__
#define __FE_FASTEMBEDDED_OS_MAIL_H__

#include <stdint.h>
#include <assert.h>

#include "FreeRTOS.h"
#include "semphr.h"

namespace fastembedded {
class OSMailPrivateData {
public:
	xQueueHandle m_msg;
};

/**
 * Mail used to save big item dynamic allocate,
 * user must new/delete it's mail item else memory
 * will leak.
 */
template<class mail, int len = 16>
class OSMail {
public:

	template<class mail, int len>
	OSMail() {
		this->d_ptr = new OSMailPrivateData();
		assert(this->d_ptr);
		this->d_ptr->m_msg = xQueueCreate(len, sizeof(void *));
		assert(this->d_ptr->m_msg);
	}

	virtual ~OSMail()
	{
		vQueueDelete(this->d_ptr->m_msg);
		delete this->d_ptr;
	}

	template<class mail, int len>
	void put(mail *item)
	{
		// if(interrupt)
		// {
		// }
		// else
		xQueueSend(this->d_ptr->m_msg, &item, portMAX_DELAY);
	}

	template<class mail, int len>
	bool tryPut(mail *item, uint32_t ms = 0)
	{
		portTickType ticks;
		ticks = ms / portTICK_RATE_MS;
		if (ticks == 0) {
			ticks = 1;
		}

		// if(interrupt)
		// {
		// }
		// else
		if (xQueueSend(this->d_ptr->m_msg, &item, ticks) != pdTRUE) {
			return false;
		}
		return true;
	}

	template<class mail, int len>
	mail *get()
	{
		mail *item;
		// if(interrupt)
		// {
		// }
		// else
		xQueueReceive(this->d_ptr->m_msg, &item, portMAX_DELAY);
		return item;
	}

	template<class mail, int len>
	mail *tryGet(uint32_t ms = 0)
	{
		mail *item;
		portTickType ticks;
		ticks = ms / portTICK_RATE_MS;
		if (ticks == 0) {
			ticks = 1;
		}

		// if(interrupt)
		// {
		// }
		// else
		if (xQueueSend(this->d_ptr->m_msg, &item, ticks) != pdTRUE) {
			return NULL;
		}
		return item;
	}
private:
	OSMailPrivateData *d_ptr;
};

} /* namespace fastembedded */

#endif /* MUTEX_H_ */
