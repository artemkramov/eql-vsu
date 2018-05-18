//
//  Values are 32 bit values layed out as follows:
//
//   3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
//   1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//  +---+-+-+-----------------------+-------------------------------+
//  |Sev|C|R|     Facility          |               Code            |
//  +---+-+-+-----------------------+-------------------------------+
//
//  where
//
//      Sev - is the severity code
//
//          00 - Success
//          01 - Informational
//          10 - Warning
//          11 - Error
//
//      C - is the Customer code flag
//
//      R - is a reserved bit
//
//      Facility - is the facility code
//
//      Code - is the facility's status code
//
//
// Define the facility codes
//


//
// Define the severity codes
//


//
// MessageId: st_no_session
//
// MessageText:
//
//  ����� � �������� ��������� �� �����������%0
//
#define st_no_session                    ((DWORD)0xC0005001L)

//
// MessageId: st_syntax_error
//
// MessageText:
//
//  �������������� ������ � ������: %0
//
#define st_syntax_error                  ((DWORD)0xC0005002L)

//
// MessageId: st_invalid_plu_code
//
// MessageText:
//
//  �������� ��� ������: %0
//
#define st_invalid_plu_code              ((DWORD)0xC0005003L)

//
// MessageId: st_invalid_plu_name
//
// MessageText:
//
//  �������� ��� ������: %0
//
#define st_invalid_plu_name              ((DWORD)0xC0005004L)

//
// MessageId: st_invalid_plu_price
//
// MessageText:
//
//  �������� ����: %0
//
#define st_invalid_plu_price             ((DWORD)0xC0005005L)

//
// MessageId: st_invalid_plu_amount
//
// MessageText:
//
//  �������� ����������: %0
//
#define st_invalid_plu_amount            ((DWORD)0xC0005006L)

//
// MessageId: st_invalid_plu_dept
//
// MessageText:
//
//  �������� �����: %0
//
#define st_invalid_plu_dept              ((DWORD)0xC0005007L)

//
// MessageId: st_invalid_plu_group
//
// MessageText:
//
//  �������� �������� ������: %0
//
#define st_invalid_plu_group             ((DWORD)0xC0005008L)

//
// MessageId: st_invalid_plu_tax
//
// MessageText:
//
//  �������� ��������� ������: %0
//
#define st_invalid_plu_tax               ((DWORD)0xC0005009L)

//
// MessageId: st_invalid_comment
//
// MessageText:
//
//  �������� ������� ����������: %0
//
#define st_invalid_comment               ((DWORD)0xC000500AL)

//
// MessageId: st_invalid_payment_kind
//
// MessageText:
//
//  �������� ��� ������: %0
//
#define st_invalid_payment_kind          ((DWORD)0xC000500BL)

//
// MessageId: st_invalid_payment_sum
//
// MessageText:
//
//  �������� ����� ������: %0
//
#define st_invalid_payment_sum           ((DWORD)0xC000500CL)

//
// MessageId: st_invalid_payment_code
//
// MessageText:
//
//  �������� �������������� ���: %0
//
#define st_invalid_payment_code          ((DWORD)0xC000500DL)

//
// MessageId: st_invalid_discount_kind
//
// MessageText:
//
//  �������� ��� ������: %0
//
#define st_invalid_discount_kind         ((DWORD)0xC000500EL)

//
// MessageId: st_invalid_discount_sum
//
// MessageText:
//
//  �������� ����� ������: %0
//
#define st_invalid_discount_sum          ((DWORD)0xC000500FL)

//
// MessageId: st_invalid_inout_kind
//
// MessageText:
//
//  �������� ��� �����/������: %0
//
#define st_invalid_inout_kind            ((DWORD)0xC0005010L)

//
// MessageId: st_invalid_inout_sum
//
// MessageText:
//
//  �������� ����� �����/������: %0
//
#define st_invalid_inout_sum             ((DWORD)0xC0005011L)

//
// MessageId: st_invalid_cmdid
//
// MessageText:
//
//  ����������� ������������� ������� � ������: %0
//
#define st_invalid_cmdid                 ((DWORD)0xC0005012L)

//
// MessageId: st_invalid_plu_flags
//
// MessageText:
//
//  �������� ������ ������: %0
//
#define st_invalid_plu_flags             ((DWORD)0xC0005013L)

//
// MessageId: st_invalid_bin_id
//
// MessageText:
//
//  ������������ ������������� ��������� �����������: %0
//
#define st_invalid_bin_id                ((DWORD)0xC0005014L)

//
// MessageId: st_invalid_bin_data
//
// MessageText:
//
//  ������������ ������ ��������� �����������: %0
//
#define st_invalid_bin_data              ((DWORD)0xC0005015L)

