#include "Core/String/FString.h"


namespace ishak {

	String::String()
	{
		m_size = 1;
		m_buffer = new char[m_size];
		m_buffer[0] = nullChar;
	}

	String::~String()
	{
		delete [] m_buffer;		
		m_buffer = nullptr;
	}

	const char* String::c_str()
	{
		return m_buffer;
	}

	const char* String::c_str() const
	{
		return m_buffer;
	}


	bool String::IsEmpty() const
	{
		if(m_buffer)
		{
			return m_buffer[0] == nullChar;
		}

		return true;
	}

	String::String(const char* word)
	{
		// Adding +1 for the "/0" char that indicates the end of the string, for memory.	
		m_size = strlen(word) + 1;

		// Allocating memory for the strPtr first
		m_buffer = new char[m_size];

		// Memory copy for the strings. This will copy the bytes pointed by the word* to strPtr*, value copy basically.
		memcpy(m_buffer, word, m_size);
	}


	String::String(String&& other) noexcept
	{
		m_buffer = other.m_buffer;
		m_size = other.m_size;

		other.m_buffer = nullptr;
		other.m_size = 0;
	}

	String::String(const String& other) noexcept
	{
		m_size = other.m_size;
		m_buffer = new char[m_size];
		memcpy(m_buffer, other.m_buffer, m_size);
	}

	String& String::operator=(const String& other) noexcept
	{
		if(this != &other)
		{
			delete[] m_buffer;

			m_size = other.m_size;
			
			m_buffer = new char[m_size];
			memcpy(m_buffer, other.m_buffer, m_size);
		}

		return *this;
	}

	String& String::operator = (String&& other) noexcept
	{
		if(this != &other)
		{
			delete[] m_buffer;

			m_buffer = other.m_buffer;
			m_size = other.m_size;

			other.m_buffer = nullptr;
			other.m_size = 0;
		}

		return *this;
	}

	String String::operator + (const String& other) noexcept
	{
		size_t finalSize{ other.m_size + m_size };		
		// As 2 strings, we have to subtract 1 null character.
		char* finalBuffer = new char[finalSize - 1];
				
		// This is to keep track of the final buffer as we copy the data from the 
		// other 2 strings
		size_t addingIdx = 0;
		for(size_t thisIdx = 0; thisIdx < m_size; thisIdx++)
		{
			char letter{ m_buffer[thisIdx]};

			// Exclude the null char. The other string's buffer will do it.
			if(letter == nullChar)
			{
				addingIdx = thisIdx;
				continue;
			}

			finalBuffer[thisIdx] = letter;
		}


		for (size_t otherIdx = 0; otherIdx < other.m_size; otherIdx++, addingIdx++)
		{
			char letter{ other.m_buffer[otherIdx] };
			finalBuffer[addingIdx] = letter;
		}
		

		return String{ finalBuffer };
	}

	String String::operator+(const String& other) const noexcept
	{
		size_t finalSize{ other.m_size + m_size };
		// As 2 strings, we have to subtract 1 null character.
		char* finalBuffer = new char[finalSize - 1];

		// This is to keep track of the final buffer as we copy the data from the 
		// other 2 strings
		size_t addingIdx = 0;
		for (size_t thisIdx = 0; thisIdx < m_size; thisIdx++)
		{
			char letter{ m_buffer[thisIdx] };

			// Exclude the null char. The other string's buffer will do it.
			if (letter == nullChar)
			{
				addingIdx = thisIdx;
				continue;
			}

			finalBuffer[thisIdx] = letter;
		}


		for (size_t otherIdx = 0; otherIdx < other.m_size; otherIdx++, addingIdx++)
		{
			char letter{ other.m_buffer[otherIdx] };
			finalBuffer[addingIdx] = letter;
		}


		return String{ finalBuffer };
	}

	String String::operator + (const char* other) noexcept
	{
		// the strlen(const char*) does not count with the null character.
		size_t otherSize{ strlen(other) };
		size_t finalSize{ otherSize + m_size};

		// As the const char* does not count with the nullchar and this string does, then just 
		// allocate the result size.
		char* finalBuffer = new char[finalSize];

		// This is to keep track of the final buffer as we copy the data from the 
		// other 2 strings
		size_t addingIdx = 0;
		for (size_t thisIdx = 0; thisIdx < m_size; thisIdx++)
		{
			char letter{ m_buffer[thisIdx] };

			// Exclude the null char. The other string's buffer will do it.
			if (letter == nullChar)
			{
				addingIdx = thisIdx;
				continue;
			}

			finalBuffer[thisIdx] = letter;
		}


		// Going all the way to the null character to finish the word, that is why we add +1.
		for (size_t otherIdx = 0; otherIdx < otherSize + 1; otherIdx++, addingIdx++)
		{
			char letter{ other[otherIdx] };
			finalBuffer[addingIdx] = letter;
		}


		return String{ finalBuffer };
	}

	String String::operator + (const char* other) const noexcept
	{
		// the strlen(const char*) does not count with the null character.
		size_t otherSize{ strlen(other) };
		size_t finalSize{ otherSize + m_size };

		// As the const char* does not count with the nullchar and this string does, then just 
		// allocate the result size.
		char* finalBuffer = new char[finalSize];

		// This is to keep track of the final buffer as we copy the data from the 
		// other 2 strings
		size_t addingIdx = 0;
		for (size_t thisIdx = 0; thisIdx < m_size; thisIdx++)
		{
			char letter{ m_buffer[thisIdx] };

			// Exclude the null char. The other string's buffer will do it.
			if (letter == nullChar)
			{
				addingIdx = thisIdx;
				continue;
			}

			finalBuffer[thisIdx] = letter;
		}


		// Going all the way to the null character to finish the word, that is why we add +1.
		for (size_t otherIdx = 0; otherIdx < otherSize + 1; otherIdx++, addingIdx++)
		{
			char letter{ other[otherIdx] };
			finalBuffer[addingIdx] = letter;
		}


		return String{ finalBuffer };
	}

	bool String::operator==(const String& other) noexcept
	{
		bool isEqual{ strcmp(m_buffer, other.m_buffer) == 0 };
		return strcmp(m_buffer, other.m_buffer) == 0;		
	}

	void String::operator +=(const String& other) noexcept
	{
		// Substracting 1 null operator, since when merged the both strings, we will have just one at the end 
		// of this string.
		const size_t totalSize = (m_size + other.m_size) - 1;		

		// Allocate the new buffer
		char* newBuffer= new char[totalSize];

		// Copy the bytes directly to the new buffer from the old one.
		std::memcpy(newBuffer, m_buffer, m_size - 1);

		// Copy the other string directly from the end of the first one.
		std::memcpy(newBuffer + m_size - 1, other.m_buffer, other.m_size);

		delete[] m_buffer;
		m_buffer = nullptr;
		
		m_buffer = newBuffer;
		m_size = totalSize;
	}

	bool String::Find(const char* toFind)
	{
		if (!m_buffer || !toFind)
		{
			return false;
		}		

		const size_t otherSize{ strlen(toFind) };
		

		bool bSameLastCheck{ false };
		size_t lettersThatWereTheSame{ 0 };
		size_t foundIdx{ 0 };
		for (int thisIdx = 0; thisIdx < m_size; ++thisIdx)
		{
			const char thisLetter{ m_buffer[thisIdx] };

			for (size_t otherIdx = foundIdx; otherIdx < strlen(toFind); ++otherIdx)
			{
				const char otherLetter{ toFind[otherIdx] };				
				if(thisLetter == otherLetter)
				{
					lettersThatWereTheSame++;
					bSameLastCheck = true;
					foundIdx = otherIdx + 1;
					break;
				}
				else 
				{
					lettersThatWereTheSame = 0;
					bSameLastCheck = false;
					foundIdx = 0;
					break;
				}
			}

			if(bSameLastCheck && lettersThatWereTheSame == otherSize)
			{
				return true;
			}
		}


		return false;
	}


}// ishak
