/*
Copyright 2022 daywithstars

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#include <SDL_log.h>

#include <PT_ObjectEntryConverter.h>


SDL_bool PT_ObjectEntryValidate( json_object_entry entry ) {
	if ( !entry.value )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_ObjectEntryValidate: Invalid entry\n");
		return SDL_FALSE;
	}
	if ( entry.value->type == json_none ||
		entry.value->type == json_null ||
		entry.value->type == json_object ||
		entry.value->type == json_array
		)
	{
		return SDL_FALSE;
	}
	
	return SDL_TRUE;
}//PT_ObjectEntryValidate

PT_String* PT_ObjectEntryConverter_PT_String( json_object_entry entry ) {
	if ( !PT_ObjectEntryValidate(entry) )
	{	
		return NULL;
	}
	
	PT_String* pt_string = PT_StringCreate();
	if ( !pt_string )
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "PT: PT_ObjectEntryConverter_PT_String!\n");
		return NULL;
	}
	
	if ( entry.value->type == json_string )
	{
		if ( !PT_StringInsert(&pt_string, entry.value->u.string.ptr, 0) )
		{
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
			"PT: PT_ObjectEntryConverter_PT_String!\n");
			
			PT_StringDestroy(pt_string);
			return NULL;	
		}
	}
	
	return pt_string;
}//PT_ObjectEntryConverter_PT_String






