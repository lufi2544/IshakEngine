#include "Core/String/FString.h"


namespace ishak {

	String::String()
	{
		AllocateNullChar();
	}

	String::~String()
	{

	}

	void String::AllocateNullChar()
	{
		m_size = 0;
		m_buffer.Add(nullChar);
	}

	const char* String::c_str()
	{
		return m_buffer.GetData();
	}

	const char* String::c_str() const
	{
		return m_buffer.GetData();
	}


	bool String::IsEmpty() const
	{
		return m_size == 0 && m_buffer[0] == nullChar;
	}

	String::String(const char* word)
	{
		m_size = strlen(word);

		// Adding +1 for the "/0" char that indicates the end of the string, for memory.	
		// Allocating memory for the strPtr first
		m_buffer.AddDefaulted(m_size + 1);

		// Memory copy for the strings. This will copy the bytes pointed by the word* to strPtr*, value copy basically.

		for(int i = 0; i < m_size; ++i)
		{
			m_buffer[i] = word[i];
		}

		// Adding the nullchar at the end

		m_buffer[m_size] = nullChar;

	}


	String::String(String&& other) noexcept
	{
		m_buffer = other.m_buffer;
		m_size = other.m_size;

		other.m_buffer.Clear();
		other.m_buffer.Add(nullChar);
		other.m_size = 0;
	}


	String::String(const String& other) noexcept
	{
		m_size = other.m_size;
		m_buffer.AddDefaulted(other.m_buffer.Size());
		for(int i = 0; i < m_buffer.Size(); ++i)
		{
			m_buffer[i] = other.m_buffer[i];
		}

	}

	String& String::operator =(const	String& other) noexcept
	{
		if(this != &other)
		{
			m_buffer.Clear();
			m_size = other.m_size;			
			m_buffer.AddDefaulted(other.m_buffer.Size());

			for(int i = 0; i < m_buffer.Size(); ++i)
			{
				m_buffer[i] = other.m_buffer[i];
			}
		}

		return *this;
	}

	String& String::operator = (String&& other) noexcept
	{
		if(this != &other)
		{
			if(!m_buffer.IsEmpty())
			{
				m_buffer.Clear();
			}

			m_buffer = other.m_buffer;
			m_size = other.m_size;

			// Set the other string to be empty.
			other.Clear();
		}

		return *this;
	}

	String String::operator + (const String& other) noexcept
	{
		size_t newBufferSize{ (other.m_buffer.Size() + m_buffer.Size()) - 1 /* Subtracting 1 null char */};

		TArray<char> newBuffer;
		newBuffer.AddDefaulted(newBufferSize);
				
		// This is to keep track of the final buffer as we copy the data from the 
		// other 2 strings
		size_t nullCharIdx = m_buffer.Size() - 1;
		for(size_t thisIdx = 0; thisIdx < nullCharIdx; thisIdx++)
		{
			char letter{ m_buffer[thisIdx]};

			// Exclude the null char. The other string's buffer will do it.
			newBuffer[thisIdx] = letter;
		}


		for (size_t otherIdx = 0; otherIdx < other.m_buffer.Size(); otherIdx++)
		{
			char letter{ other.m_buffer[otherIdx] };

			// could optimise this addition by creating another idx and just adding 1 every loop beat
			newBuffer[nullCharIdx + otherIdx] = letter;
		}
		
		String newString;
		newString.m_buffer = newBuffer;
		newString.m_size = newBufferSize - 1;

		return newString;
	}

	String String::operator+(const String& other) const noexcept
	{
		size_t newBufferSize{ (other.m_buffer.Size() + m_buffer.Size()) - 1 /* Subtracting 1 null char */ };

		TArray<char> newBuffer;
		newBuffer.AddDefaulted(newBufferSize);

		// This is to keep track of the final buffer as we copy the data from the 
		// other 2 strings
		size_t nullCharIdx = m_buffer.Size() - 1;
		for (size_t thisIdx = 0; thisIdx < nullCharIdx; thisIdx++)
		{
			char letter{ m_buffer[thisIdx] };

			// Exclude the null char. The other string's buffer will do it.
			newBuffer[thisIdx] = letter;
		}


		for (size_t otherIdx = 0; otherIdx < other.m_buffer.Size(); otherIdx++)
		{
			char letter{ other.m_buffer[otherIdx] };

			// could optimise this addition by creating another idx and just adding 1 every loop beat
			newBuffer[nullCharIdx + otherIdx] = letter;
		}

		String newString;
		newString.m_buffer = newBuffer;
		newString.m_size = newBufferSize - 1;

		return newString;
	}

	String String::operator + (const char* other) noexcept
	{
		// the strlen(const char*) does not count with the null character.
		const size_t otherSize{ strlen(other) };
		const size_t finalBufferSize{ otherSize + m_buffer.Size()};

		// As the const char* does not count with the nullchar and this string does, then just 
		// allocate the result size.
		TArray<char> newBuffer;
		newBuffer.AddDefaulted(finalBufferSize);

		// This is to keep track of the final buffer as we copy the data from the 
		// other 2 strings
		const size_t thisNullCharIdx = m_buffer.Size() - 1;
		for (size_t thisIdx = 0; thisIdx < thisNullCharIdx; thisIdx++)
		{
			char letter{ m_buffer[thisIdx] };
			newBuffer[thisIdx] = letter;
		}


		// Going all the way to the null character to finish the word, that is why we add +1.
		for (size_t otherIdx = 0; otherIdx < otherSize + 1; otherIdx++)
		{
			char letter{ other[otherIdx] };
			newBuffer[thisNullCharIdx + otherIdx] = letter;
		}

		String newString;
		newString.m_buffer = newBuffer;
		newString.m_size = finalBufferSize - 1;

		return newString;
	}

	String String::operator + (const char* other) const noexcept
	{
		// the strlen(const char*) does not count with the null character.
		const size_t otherSize{ strlen(other) };
		const size_t finalBufferSize{ otherSize + m_buffer.Size() };

		// As the const char* does not count with the nullchar and this string does, then just 
		// allocate the result size.
		TArray<char> newBuffer;
		newBuffer.AddDefaulted(finalBufferSize);

		// This is to keep track of the final buffer as we copy the data from the 
		// other 2 strings
		const size_t thisNullCharIdx = m_buffer.Size() - 1;
		for (size_t thisIdx = 0; thisIdx < thisNullCharIdx; thisIdx++)
		{
			char letter{ m_buffer[thisIdx] };
			newBuffer[thisIdx] = letter;
		}


		// Going all the way to the null character to finish the word, that is why we add +1.
		for (size_t otherIdx = 0; otherIdx < otherSize + 1; otherIdx++)
		{
			char letter{ other[otherIdx] };
			newBuffer[thisNullCharIdx + otherIdx] = letter;
		}

		String newString;
		newString.m_buffer = newBuffer;
		newString.m_size = finalBufferSize - 1;

		return newString;
	}

	bool String::operator==(const String& other) noexcept
	{
		return m_buffer == other.m_buffer;
	}

	void String::operator +=(const String& other) noexcept
	{
		String newSring = *this + other;
		*this = newSring;
	}

	bool String::Find(const char* toFind)
	{
		if (IsEmpty() || !toFind)
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


	void String::Clear()
	{
		m_size = 0;
		m_buffer.Clear();
		m_buffer.Add(nullChar);
	}


}// ishak
