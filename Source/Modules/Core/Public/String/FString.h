#pragma once

#include "CoreMinimal.h"
#include "CoreConfig.h"

namespace ishak {

	struct CORE_API String
	{			
		const char nullChar = '\0';
		String();
		String(const char* word);

		String(String&& other) noexcept;
		String(const String& other) noexcept;

		String& operator = (const String& other) noexcept ;
		String& operator = (String&& other) noexcept;

		String operator + (const String& other) noexcept;
		String operator + (const char* other) noexcept;

		bool operator == (const String& other) noexcept;

		void operator += (const String& other) noexcept;

		~String();

		const char* c_str();		
		bool IsEmpty() const;
		bool Find(const char* toFind);

	private:
		/* Character buffer.*/
		char* m_buffer{ nullptr };

		/* String size. */
		size_t m_size{ 0 };
	};


}// ishak