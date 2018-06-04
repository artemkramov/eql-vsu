#ifndef __MESSAGES_H__
#define __MESSAGES_H__

////////////////////////////////////////
// Eventlog categories
//
// Categories always have to be the first entries in a message file!
//
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
// MessageId: 0xC0007000L (No symbolic name defined)
//
// MessageText:
//
//  Utility message 7000%0
//


//
// MessageId: VSU_PriceNotParsed
//
// MessageText:
//
//  Can't parse price field%0
//
#define VSU_PriceNotParsed               0xC0007001L

//
// MessageId: VSU_QtyNotParsed
//
// MessageText:
//
//  Can't parse quantity field%0
//
#define VSU_QtyNotParsed                 0xC0007002L    

//
// MessageId: VSU_TaxNotParsed
//
// MessageText:
//
//  Can't parse tax field%0
//
#define VSU_TaxNotParsed                 0xC0007003L    

//
// MessageId: VSU_CodeNotParsed
//
// MessageText:
//
//  Can't parse code field%0
//
#define VSU_CodeNotParsed                0xC0007004L    

//
// MessageId: VSU_SpaceNotParsed
//
// MessageText:
//
//  Can't parse space field%0
//
#define VSU_SpaceNotParsed               0xC0007005L    

//
// MessageId: VSU_NameNotParsed
//
// MessageText:
//
//  Can't parse name field%0
//
#define VSU_NameNotParsed                0xC0007006L    

//
// MessageId: VSU_PayKindNotParsed
//
// MessageText:
//
//  Can't parse payment kind%0
//
#define VSU_PayKindNotParsed             0xC0007007L    

//
// MessageId: VSU_PaySumNotParsed
//
// MessageText:
//
//  Can't parse payment sum%0
//
#define VSU_PaySumNotParsed              0xC0007008L    

//
// MessageId: VSU_TextNotParsed
//
// MessageText:
//
//  Can't parse comment text%0
//
#define VSU_TextNotParsed                0xC0007009L    

//
// MessageId: VSU_SyntaxError
//
// MessageText:
//
//  Syntax error. Line is improperly formatted%0
//
#define VSU_SyntaxError                  0xC000700AL    

//
// MessageId: VSU_DiscKindNotParsed
//
// MessageText:
//
//  Can't parse discount type field%0
//
#define VSU_DiscKindNotParsed            0xC000700BL    

//
// MessageId: VSU_DiscPercNotParsed
//
// MessageText:
//
//  Can't parse percent field%0
//
#define VSU_DiscPercNotParsed            0xC000700CL    

//
// MessageId: VSU_DeptNotParsed
//
// MessageText:
//
//  Can't parse department field%0
//
#define VSU_DeptNotParsed                0xC000700DL    

//
// MessageId: VSU_2DepNotParsed
//
// MessageText:
//
//  Can't parse group field%0
//
#define VSU_2DepNotParsed                0xC000700EL    

//
// MessageId: VSU_2GrpNotParsed
//
// MessageText:
//
//  Can't parse subgroup field%0
//
#define VSU_2GrpNotParsed                0xC000700FL    

//
// MessageId: VSU_OnlyBeforeReceipt
//
// MessageText:
//
//  '%1!.*s!' command allowed only before receipt start printing%0
//
#define VSU_OnlyBeforeReceipt            0xC0007010L    

//
// MessageId: VSU_ECRError
//
// MessageText:
//
//  Error from cash register: %1
//
#define VSU_ECRError                     0xC0007011L    

//
// MessageId: VSU_WrongCashierName
//
// MessageText:
//
//  Wrong cashier name%0
//
#define VSU_WrongCashierName             0xC0007012L    

//
// MessageId: VSU_CannotConnectAs
//
// MessageText:
//
//  Cannot connect as specified cashier. Close another program connected as this cashier%0
//
#define VSU_CannotConnectAs              0xC0007013L    

//
// MessageId: VSU_FactKindNotParsed
//
// MessageText:
//
//  Can't parse facture's data type%0
//
#define VSU_FactKindNotParsed            0xC0007014L    

//
// MessageId: VSU_FactDanchNotParsed
//
// MessageText:
//
//  Can't parse facture's tax code%0
//
#define VSU_FactDanchNotParsed           0xC0007015L    

//
// MessageId: 0xC0005000L (No symbolic name defined)
//
// MessageText:
//
//  Utility message 5000%0
//


//
// MessageId: st_no_session
//
// MessageText:
//
//  Connection with Cash register is not established%0
//
#define st_no_session                    0xC0005001L

//
// MessageId: st_syntax_error
//
// MessageText:
//
//  Syntax error in line: %0
//
#define st_syntax_error                  0xC0005002L

//
// MessageId: st_invalid_plu_code
//
// MessageText:
//
//  Invalid PLU code: %0
//
#define st_invalid_plu_code              0xC0005003L

//
// MessageId: st_invalid_plu_name
//
// MessageText:
//
//  Invalid command: %0
//
#define st_invalid_plu_name              0xC0005004L

//
// MessageId: st_invalid_plu_price
//
// MessageText:
//
//  Invalid price of selling: %0
//
#define st_invalid_plu_price             0xC0005005L

//
// MessageId: st_invalid_plu_amount
//
// MessageText:
//
//  Invalid amount of selling: %0
//
#define st_invalid_plu_amount            0xC0005006L

//
// MessageId: st_invalid_plu_dept
//
// MessageText:
//
//  Invalid dept of selling: %0
//
#define st_invalid_plu_dept              0xC0005007L

//
// MessageId: st_invalid_plu_group
//
// MessageText:
//
//  Invalid group of selling: %0
//
#define st_invalid_plu_group             0xC0005008L

//
// MessageId: st_invalid_plu_tax
//
// MessageText:
//
//  Invalid tax of selling: %0
//
#define st_invalid_plu_tax               0xC0005009L

//
// MessageId: st_invalid_comment
//
// MessageText:
//
//  Invalid comment command: %0
//
#define st_invalid_comment               0xC000500AL

//
// MessageId: st_invalid_payment_kind
//
// MessageText:
//
//  Invalid kind of payment: %0
//
#define st_invalid_payment_kind          0xC000500BL

//
// MessageId: st_invalid_payment_sum
//
// MessageText:
//
//  Invalid sum of payment: %0
//
#define st_invalid_payment_sum           0xC000500CL

//
// MessageId: st_invalid_payment_code
//
// MessageText:
//
//  Invalid custom code: %0
//
#define st_invalid_payment_code          0xC000500DL

//
// MessageId: st_invalid_discount_kind
//
// MessageText:
//
//  Invalid kind of discount: %0
//
#define st_invalid_discount_kind         0xC000500EL

//
// MessageId: st_invalid_discount_sum
//
// MessageText:
//
//  Invalid sum of discount: %0
//
#define st_invalid_discount_sum          0xC000500FL

//
// MessageId: st_invalid_inout_kind
//
// MessageText:
//
//  Invalid kind of deposit/withdrawal: %0
//
#define st_invalid_inout_kind            0xC0005010L

//
// MessageId: st_invalid_inout_sum
//
// MessageText:
//
//  Invalid sum of deposit/withdrawal: %0
//
#define st_invalid_inout_sum             0xC0005011L

//
// MessageId: st_invalid_cmdid
//
// MessageText:
//
//  Unknown command identifier in line: %0
//
#define st_invalid_cmdid                 0xC0005012L

//
// MessageId: st_invalid_plu_flags
//
// MessageText:
//
//  Invalid flags of selling: %0
//
#define st_invalid_plu_flags             0xC0005013L

//
// MessageId: st_invalid_bin_id
//
// MessageText:
//
//  Invalid binary comment identifier: %0
//
#define st_invalid_bin_id                0xC0005014L

//
// MessageId: st_invalid_bin_data
//
// MessageText:
//
//  Invalid binary comment format: %0
//
#define st_invalid_bin_data              0xC0005015L

//
// MessageId: st_invalid_vatfact_code
//
// MessageText:
//
//  Invalid VAT Facture code: %0
//
#define st_invalid_vatfact_code          0xC0005016L

//
// MessageId: st_no_BegVatFact
//
// MessageText:
//
//  Cash register does not support facture printing: %0
//
#define st_no_BegVatFact                 0xC0005017L

//
// MessageId: st_no_BegReturn
//
// MessageText:
//
//  Cash register does not support return receipts: %0
//
#define st_no_BegReturn                  0xC0005018L

//
// MessageId: st_error_handling_mode
//
// MessageText:
//
//  Error handling mode
//
#define st_error_handling_mode           0xC0005019L

//
// MessageId: st_no_session_started
//
// MessageText:
//
//  Can not handle '%1!ls!' because connection with cash register is not initiated%0
//
#define st_no_session_started            0xC000501AL

//
// MessageId: st_no_session_established
//
// MessageText:
//
//  Can not handle '%1!ls!' because VSU is waiting for cash register connection%0
//
#define st_no_session_established        0xC000501BL

//
// MessageId: st_temporary_unavailable
//
// MessageText:
//
//  Can not handle '%1!ls!' because cash register has suddenly disconnected%0
//
#define st_temporary_unavailable         0xC000501CL


#endif  //__MESSAGES_H__
