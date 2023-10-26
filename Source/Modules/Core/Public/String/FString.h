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
		String operator + (const String& other) const noexcept;
		String operator + (const char* other) noexcept;
		String operator + (const char* other) const noexcept;

		bool operator == (const String& other) noexcept;

		char* begin() 
		{
			return &m_buffer[0];
		}

		char* end() 
		{
			return &m_buffer[m_size - 1];
		}		

		friend bool operator == (const String& lhs, const String& rhs)
		{
			return strcmp(lhs.m_buffer, rhs.m_buffer) == 0;
		}
		void operator += (const String& other) noexcept;

		~String();

		const char* c_str();		
		const char* c_str() const; 
		bool IsEmpty() const;
		bool Find(const char* toFind);

	private:
		/* Character buffer.*/
		char* m_buffer{ nullptr };

		/* String size. */
		size_t m_size{ 0 };
	};


}// ishak

	// Implementation for now for the std::map
namespace std
{
	template<>
	struct hash<ishak::String>
	{
		size_t operator()(const ishak::String& str) const
		{
			size_t h1 = std::hash<const char*>()(str.c_str());
			return h1 << 1;
		}

	};
}