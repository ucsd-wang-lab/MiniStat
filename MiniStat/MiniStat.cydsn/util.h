///* ========================================
// *
// * NANOBIOELECTRONICS LAB UCSD, 2019
// * All Rights Reserved
// * UNPUBLISHED, LICENSED SOFTWARE.
// *
// * CONFIDENTIAL AND PROPRIETARY INFORMATION
// * WHICH IS THE PROPERTY OF the company.
// *
// * ========================================
// */
//
///* Note that the file assumes UART_Start has been called in main and 
//   heap has been allocated a space of at least 0x400 bytes */
//
//#include <project.h>
//#include <stdio.h>
//
//#if defined(__ARMCC_VERSION)
//    
///* For MDK/RVDS compiler revise fputc function for printf functionality */
//struct __FILE 
//{
//    int handle;  
//};
// 
//enum 
//{
//    STDIN_HANDLE,
//    STDOUT_HANDLE,
//    STDERR_HANDLE
//};
// 
//FILE __stdin = {STDIN_HANDLE};
//FILE __stdout = {STDOUT_HANDLE};
//FILE __stderr = {STDERR_HANDLE};
// 
//int fputc(int ch, FILE *file) 
//{
//    int ret = EOF;
// 
//    switch( file->handle )
//    {
//        case STDOUT_HANDLE:
//            UART_UartPutChar(ch);
//            ret = ch ;
//            break ;
// 
//        case STDERR_HANDLE:
//            ret = ch ;
//            break ;
// 
//        default:
//            file = file;
//            break ;
//    }
//    return ret ;
//}
// 
//#elif defined (__ICCARM__)      /* IAR */
// 
///* For IAR compiler revise __write() function for printf functionality */
//size_t __write(int handle, const unsigned char * buffer, size_t size)
//{
//    size_t nChars = 0;
// 
//    if (buffer == 0)
//    {
//        /*
//         * This means that we should flush internal buffers.  Since we
//         * don't we just return.  (Remember, "handle" == -1 means that all
//         * handles should be flushed.)
//         */
//        return (0);
//    }
// 
//    for (/* Empty */; size != 0; --size)
//    {
//        UART_UartPutChar(*buffer++);
//        ++nChars;
//    }
// 
//    return (nChars);
//}
// 
//#else  /* (__GNUC__)  GCC */
// 
///* For GCC compiler revise _write() function for printf functionality */
//int _write(int file, char *ptr, int len)
//{
//    int i;
//    file = file;
//    for (i = 0; i < len; i++)
//    {
//        UART_UartPutChar(*ptr++);
//    }
//    return len;
//}
// 
// 
//#endif  /* (__ARMCC_VERSION) */