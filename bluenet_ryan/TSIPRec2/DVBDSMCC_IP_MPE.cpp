// DVBDSMCC_IP_MPE.cpp: implementation of the CDVBDSMCC_IP_MPE class.
//
//////////////////////////////////////////////////////////////////////
//  CYJ,2008-12-8 �޸� OnTableReceived, ֧�� Session ƴ��

#include <stdafx.h>
#include "DVBDSMCC_IP_MPE.h"
#include <mydvbpsi/dvbpsitablesdefine.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

static DWORD s_crc32_table[256] =
{
  0x00000000, 0x04c11db7, 0x09823b6e, 0x0d4326d9,
  0x130476dc, 0x17c56b6b, 0x1a864db2, 0x1e475005,
  0x2608edb8, 0x22c9f00f, 0x2f8ad6d6, 0x2b4bcb61,
  0x350c9b64, 0x31cd86d3, 0x3c8ea00a, 0x384fbdbd,
  0x4c11db70, 0x48d0c6c7, 0x4593e01e, 0x4152fda9,
  0x5f15adac, 0x5bd4b01b, 0x569796c2, 0x52568b75,
  0x6a1936c8, 0x6ed82b7f, 0x639b0da6, 0x675a1011,
  0x791d4014, 0x7ddc5da3, 0x709f7b7a, 0x745e66cd,
  0x9823b6e0, 0x9ce2ab57, 0x91a18d8e, 0x95609039,
  0x8b27c03c, 0x8fe6dd8b, 0x82a5fb52, 0x8664e6e5,
  0xbe2b5b58, 0xbaea46ef, 0xb7a96036, 0xb3687d81,
  0xad2f2d84, 0xa9ee3033, 0xa4ad16ea, 0xa06c0b5d,
  0xd4326d90, 0xd0f37027, 0xddb056fe, 0xd9714b49,
  0xc7361b4c, 0xc3f706fb, 0xceb42022, 0xca753d95,
  0xf23a8028, 0xf6fb9d9f, 0xfbb8bb46, 0xff79a6f1,
  0xe13ef6f4, 0xe5ffeb43, 0xe8bccd9a, 0xec7dd02d,
  0x34867077, 0x30476dc0, 0x3d044b19, 0x39c556ae,
  0x278206ab, 0x23431b1c, 0x2e003dc5, 0x2ac12072,
  0x128e9dcf, 0x164f8078, 0x1b0ca6a1, 0x1fcdbb16,
  0x018aeb13, 0x054bf6a4, 0x0808d07d, 0x0cc9cdca,
  0x7897ab07, 0x7c56b6b0, 0x71159069, 0x75d48dde,
  0x6b93dddb, 0x6f52c06c, 0x6211e6b5, 0x66d0fb02,
  0x5e9f46bf, 0x5a5e5b08, 0x571d7dd1, 0x53dc6066,
  0x4d9b3063, 0x495a2dd4, 0x44190b0d, 0x40d816ba,
  0xaca5c697, 0xa864db20, 0xa527fdf9, 0xa1e6e04e,
  0xbfa1b04b, 0xbb60adfc, 0xb6238b25, 0xb2e29692,
  0x8aad2b2f, 0x8e6c3698, 0x832f1041, 0x87ee0df6,
  0x99a95df3, 0x9d684044, 0x902b669d, 0x94ea7b2a,
  0xe0b41de7, 0xe4750050, 0xe9362689, 0xedf73b3e,
  0xf3b06b3b, 0xf771768c, 0xfa325055, 0xfef34de2,
  0xc6bcf05f, 0xc27dede8, 0xcf3ecb31, 0xcbffd686,
  0xd5b88683, 0xd1799b34, 0xdc3abded, 0xd8fba05a,
  0x690ce0ee, 0x6dcdfd59, 0x608edb80, 0x644fc637,
  0x7a089632, 0x7ec98b85, 0x738aad5c, 0x774bb0eb,
  0x4f040d56, 0x4bc510e1, 0x46863638, 0x42472b8f,
  0x5c007b8a, 0x58c1663d, 0x558240e4, 0x51435d53,
  0x251d3b9e, 0x21dc2629, 0x2c9f00f0, 0x285e1d47,
  0x36194d42, 0x32d850f5, 0x3f9b762c, 0x3b5a6b9b,
  0x0315d626, 0x07d4cb91, 0x0a97ed48, 0x0e56f0ff,
  0x1011a0fa, 0x14d0bd4d, 0x19939b94, 0x1d528623,
  0xf12f560e, 0xf5ee4bb9, 0xf8ad6d60, 0xfc6c70d7,
  0xe22b20d2, 0xe6ea3d65, 0xeba91bbc, 0xef68060b,
  0xd727bbb6, 0xd3e6a601, 0xdea580d8, 0xda649d6f,
  0xc423cd6a, 0xc0e2d0dd, 0xcda1f604, 0xc960ebb3,
  0xbd3e8d7e, 0xb9ff90c9, 0xb4bcb610, 0xb07daba7,
  0xae3afba2, 0xaafbe615, 0xa7b8c0cc, 0xa379dd7b,
  0x9b3660c6, 0x9ff77d71, 0x92b45ba8, 0x9675461f,
  0x8832161a, 0x8cf30bad, 0x81b02d74, 0x857130c3,
  0x5d8a9099, 0x594b8d2e, 0x5408abf7, 0x50c9b640,
  0x4e8ee645, 0x4a4ffbf2, 0x470cdd2b, 0x43cdc09c,
  0x7b827d21, 0x7f436096, 0x7200464f, 0x76c15bf8,
  0x68860bfd, 0x6c47164a, 0x61043093, 0x65c52d24,
  0x119b4be9, 0x155a565e, 0x18197087, 0x1cd86d30,
  0x029f3d35, 0x065e2082, 0x0b1d065b, 0x0fdc1bec,
  0x3793a651, 0x3352bbe6, 0x3e119d3f, 0x3ad08088,
  0x2497d08d, 0x2056cd3a, 0x2d15ebe3, 0x29d4f654,
  0xc5a92679, 0xc1683bce, 0xcc2b1d17, 0xc8ea00a0,
  0xd6ad50a5, 0xd26c4d12, 0xdf2f6bcb, 0xdbee767c,
  0xe3a1cbc1, 0xe760d676, 0xea23f0af, 0xeee2ed18,
  0xf0a5bd1d, 0xf464a0aa, 0xf9278673, 0xfde69bc4,
  0x89b8fd09, 0x8d79e0be, 0x803ac667, 0x84fbdbd0,
  0x9abc8bd5, 0x9e7d9662, 0x933eb0bb, 0x97ffad0c,
  0xafb010b1, 0xab710d06, 0xa6322bdf, 0xa2f33668,
  0xbcb4666d, 0xb8757bda, 0xb5365d03, 0xb1f740b4
};

DWORD DVB_GetCRC32( PBYTE pBuf, int nLen, DWORD dwLastCRC = 0xFFFFFFFF )
{
    for(int i=0; i<nLen; i++)
    {
		dwLastCRC = (dwLastCRC << 8) ^ s_crc32_table[(dwLastCRC >> 24) ^ (*pBuf ++)];
    }

	return dwLastCRC;
}

//////////////////////////////////////////////////////////////////////////
CDVBDSMCC_IP_MPE::CDVBDSMCC_IP_MPE( CUDPDataPort * pDataPort, WORD wPID )
{
	m_pDataPort = pDataPort;
	m_wPID = wPID;

	m_nDataHeaderLen = 20;		// Ŀǰֻ���� IP Э���װ��һ��IPͷ����20�ֽ�
	memset( m_abyDataBuf, 0, sizeof(m_abyDataBuf) );
	memset( m_abyEthAddr, 0, sizeof(m_abyEthAddr) );

	m_pInputDataBuf = m_abyDataBuf + sizeof(ETHERNET_FRAME);	// ����һ����̫֡��λ��
	m_nDataLen = 0;

	m_bHeaderReceived = false;			//	��ͷ���յ���
	m_wErrorCounter = 0;				//	TS packet ��������Ĵ���
	m_byExpectTSContinuityCounter = 0xff;	//	�ϴ� TS packet ��continuity counter

	m_pPreItem = NULL;			// ��һ��
	m_pNextItem = NULL;			// ��һ��
}

CDVBDSMCC_IP_MPE::~CDVBDSMCC_IP_MPE()
{
	if( m_pPreItem )
		m_pPreItem->m_pNextItem = m_pNextItem;

	if( m_pNextItem )
		m_pNextItem->m_pPreItem = m_pPreItem;
}

///-------------------------------------------------------
/// CYJ,2007-5-10
/// ��������:
///		���ӵ����
/// �������:
///		��
/// ���ز���:
///		��β�����Ӹ���
void CDVBDSMCC_IP_MPE::AppendItem( CDVBDSMCC_IP_MPE * pItem )
{
	ASSERT( pItem->m_pNextItem == NULL && pItem->m_pPreItem == NULL );

	CDVBDSMCC_IP_MPE * pTheLastItem = this;
	while( pTheLastItem->m_pNextItem )
	{
		pTheLastItem = pTheLastItem->m_pNextItem;
	}

	ASSERT( pTheLastItem && pTheLastItem->m_pNextItem == NULL );

	pTheLastItem->m_pNextItem = pItem;
	pItem->m_pPreItem = pTheLastItem;
}

///-------------------------------------------------------
/// CYJ,2007-5-10
/// ��������:
///		����һ��TS��������
/// �������:
///		��
/// ���ز���:
///		��
void CDVBDSMCC_IP_MPE::PushOneTSPacket(PDVB_TS_PACKET pPacket)
{
	bool bIsPayloadStart = pPacket->IsPayloadUnitStart();

	if( (!m_bHeaderReceived && !bIsPayloadStart) || pPacket->IsError() )
		return;					//	û�н��յ�����ͷ���Ҳ���һ�����Ŀ�ʼ��

	int nDataLen;
	PBYTE pPayloadData = pPacket->GetPayloadData( nDataLen );
	if( NULL == pPayloadData )
		return;					//	û������

	BYTE byContinuityCounter = pPacket->GetContinuityCount();

	if( bIsPayloadStart )
	{							//	���յ�һ���±�
		BYTE byPointerField = *pPayloadData;
		if( byPointerField )
			Write( pPayloadData+1, byPointerField );			//	����ԭ���ı�

		if( m_bHeaderReceived && 0 == m_wErrorCounter && m_nDataLen > 0 )	// û�з��������ҳɹ����յ�����
		{
			if( OnTableReceived() )	//  CYJ,2008-12-8 �ж��Ƿ�����Sesstion��֪ͨ���յ���
				Reset();
		}
		else
			Reset();				//	���¿�ʼ
		nDataLen -= (byPointerField+1);
		ASSERT( nDataLen > 0 );
		if( nDataLen < 0 )
			return;
		pPayloadData += ( byPointerField+1 );
		m_bHeaderReceived = true;
	}
	if( false==bIsPayloadStart && m_byExpectTSContinuityCounter != byContinuityCounter )
	{
		m_wErrorCounter ++;
		m_bHeaderReceived = false;
		return;					//	�������󣬷����˴ν���
	}
	m_byExpectTSContinuityCounter = (byContinuityCounter+1) & 0xF;

	if( false == Write( pPayloadData, nDataLen ) )
	{
		m_wErrorCounter ++;
		m_bHeaderReceived = false;		//	д������ʧ�ܣ���������
	}

	if( 0 == m_wErrorCounter && IsPSIPacketIntegral() )
	{							//	���յ�һ��������PSI��
		if( OnTableReceived() )		//  CYJ,2008-12-8 �ж��Ƿ�����Sesstion��֪ͨ���յ���
			Reset();				//	���¿�ʼ
	}
}

///-------------------------------------------------------
/// CYJ,2007-5-10
/// ��������:
///		���յ�һ�����ݱ�
/// �������:
///		��
/// ���ز���:
///		TRUE			��Ҫִ�� Reset ����
///		FALSE			���յ�һ���µ�Session������ִ�� Reset
BOOL CDVBDSMCC_IP_MPE::OnTableReceived()
{
	int nByteReceived = m_nDataLen;
	ASSERT( nByteReceived );
	PBYTE pSrcBuf = m_pInputDataBuf;
	ASSERT( pSrcBuf );
	if( NULL == pSrcBuf || nByteReceived <= 12 )	// һ��PAT��������Ҫ12�ֽ�
		return TRUE;

	ASSERT( *pSrcBuf == DVBPSI_TBLID_DATABROCAST_MPE );	// 0x3E
	if( *pSrcBuf != DVBPSI_TBLID_DATABROCAST_MPE )
	{
#ifdef __ENABLE_TRACE__
		TRACE("CDVBPSITable_IP_MPE: Not a MPE private data table, tableId=%d != 0\n", *pSrcBuf );
#endif // __ENABLE_TRACE__
		return TRUE;						//	���� MPE private data ��
	}
	WORD wSectionLen = ((pSrcBuf[1]&0xF) << 8) | pSrcBuf[2];
	if( nByteReceived < wSectionLen+3 )
	{
#ifdef __ENABLE_TRACE__
		TRACE("CDVBPSITable_IP_MPE: Bytes received %d < %d needed.\n", nByteReceived, wSectionLen );
#endif // __ENABLE_TRACE__
		return  TRUE;
	}

	if( 0x30 != (pSrcBuf[1] & 0x30 ) )
	{		// reserved, should be '11'
#ifdef __ENABLE_TRACE__
		TRACE("CDVBPSITable_IP_MPE: section_syntax_indicator and next should be 2,but actual=%d\n", (pSrcBuf[1] & 0xC0 ) >> 6 );
#endif // __ENABLE_TRACE__
		return  TRUE;
	}

	if( ( pSrcBuf[5] & 1 ) == 0 )
	{
#ifdef __ENABLE_TRACE__
		TRACE("CDVBPSITable_IP_MPE, current_next_indicator should be 1.\n");
#endif //__ENABLE_TRACE__
		return  TRUE;
	}

	unsigned char bSectionSyntaxIndicator = ( pSrcBuf[1] & 0x80 );
	unsigned char bPrivateIndicator = ( pSrcBuf[1] & 0x40 );
	unsigned char byPayloadScramblingCtrl = ( pSrcBuf[5] >> 4 ) & 3;
	unsigned char byAddrScramblingCtrl = (pSrcBuf[5] >> 2 ) & 3;
	unsigned char bLLC_SNAP_Flag = (pSrcBuf[5] & 2);

	if( bLLC_SNAP_Flag )
	{
#ifdef __ENABLE_TRACE__
		TRACE("IP_MPE: This is a LLC_SNAP protocol, abort\n");
#endif  // __ENABLE_TRACE__
		return  TRUE;
	}

	if( bSectionSyntaxIndicator )
	{
		if( DVB_GetCRC32( pSrcBuf, wSectionLen+3 ) )
		{							//	����CRC32���ڣ������ṹ��С��wSectionLen��3��
	#ifdef __ENABLE_TRACE__
			TRACE("CDVBPSITable_IP_MPE: Bad CRC32\n");
	#endif // __ENABLE_TRACE__
			return  TRUE;
		}
	}
	else
	{					// check sum
		// not implement
	}

	m_abyEthAddr[5] = pSrcBuf[3];
	m_abyEthAddr[4] = pSrcBuf[4];
	for(int i=0; i<4; i++)
	{
		m_abyEthAddr[i] = pSrcBuf[11-i];
	}

	PBYTE pDataPtr = pSrcBuf + 12;
	WORD wDataLen = wSectionLen - 9 - 4;	// 4 bytes CRC32 or checksum, but 3 bytes header(table_id and senction len)

	if( 0 == pSrcBuf[6] )		// only one section
	{
		PBYTE pEthFrameBuf = pDataPtr - sizeof(ETHERNET_FRAME);
	#ifdef _DEBUG
		ASSERT( pEthFrameBuf >= m_abyDataBuf );
	#endif
		ETHERNET_FRAME * pEthFrame = (ETHERNET_FRAME *)pEthFrameBuf;
		memcpy( &pEthFrame->Destination, m_abyEthAddr, sizeof(ETHERNET_ADDRESS) );
		OnEthernetFrame( pEthFrameBuf, wDataLen + sizeof(ETHERNET_FRAME) );

		//  CYJ,2008-12-8 ����ʣ��������һ���µ����ݱ����������ա�
		if( m_nDataLen > wSectionLen + 3 && m_pInputDataBuf[wSectionLen + 3] == DVBPSI_TBLID_DATABROCAST_MPE )
		{
			m_nDataLen = m_nDataLen-wSectionLen-3;
			memmove( m_pInputDataBuf, m_pInputDataBuf + wSectionLen + 3, m_nDataLen );
			return FALSE;			//  CYJ,2008-12-8 do not need to reset
		}
	}
	else
	{					// multiple section
		// δ�������ԣ�δ֪��ȷ��񣿹ʣ���һ�� ASSERT( FALSE );
#if 0
		ASSERT( FALSE );
		if( 0 == m_EthFrame.GetMemoryAllocated() )
			SetSectionCount( pSrcBuf[7]+1 );	// ����section����
		SetSectionNoStatus( pSrcBuf[6] );
		if( false == m_EthFrame.Write( pDataPtr, wDataLen ) )
			Reset( true );			// write data failed
		if( m_EthFrame.GetMemoryAllocated() && IsAllSectionReceived() && m_EthFrame.GetHeapBuf() )
			OnEthernetFrame( m_abyEthAddr, m_EthFrame.GetHeapBuf(), m_EthFrame.GetMemoryAllocated() );
#endif // 0
	}

	return TRUE;
}

///-------------------------------------------------------
/// CYJ,2007-5-10
/// ��������:
///		��λ��Ϣ
/// �������:
///		��
/// ���ز���:
///		��
void CDVBDSMCC_IP_MPE::Reset()
{
	m_nDataLen = 0;
	m_nDataHeaderLen = 20;		// Ŀǰֻ���� IP Э���װ��һ��IPͷ����20�ֽ�

	m_bHeaderReceived = false;			//	��ͷ���յ���
	m_wErrorCounter = 0;				//	TS packet ��������Ĵ���
}

///-------------------------------------------------------
/// CYJ,2007-5-10
/// ��������:
///		д������
/// �������:
///		pBuf			������
///		nLen			��С
/// ���ز���:
///		��
BOOL CDVBDSMCC_IP_MPE::Write( PBYTE pBuf, int nLen )
{
	if( nLen <= 0 || NULL == pBuf  )
		return FALSE;
	if( nLen + m_nDataLen > TS_MPE_MAX_ETHERNET_LEN )
		return FALSE;				// ������Χ
	memcpy( m_pInputDataBuf+m_nDataLen, pBuf, nLen );
	m_nDataLen += nLen;

	return TRUE;
}

///-------------------------------------------------------
/// CYJ,2006-9-21
/// ��������:
///		���ݰ��Ƿ�����
/// �������:
///		��
/// ���ز���:
///		true			��������
///		false			δ����
bool CDVBDSMCC_IP_MPE::IsPSIPacketIntegral()
{
	int nByteReceived = m_nDataLen;
	ASSERT( nByteReceived );
	if( nByteReceived <= 12+m_nDataHeaderLen )	// һ��MPE��������Ҫ12�ֽڣ��ټ�20�ֽڵ�IPͷ
		return false;
	WORD wSectionLen = ((m_pInputDataBuf[1]&0xf) << 8) | m_pInputDataBuf[2];
	return( nByteReceived >= wSectionLen+3 );
}

///-------------------------------------------------------
/// CYJ,2007-5-10
/// ��������:
///		���յ�һ����̫֡
/// �������:
///		��
/// ���ز���:
///		��
inline void CDVBDSMCC_IP_MPE::OnEthernetFrame( PBYTE pEthernetAddr, int nFrameLen )
{
	m_pDataPort->OnEthernetFrame( pEthernetAddr, nFrameLen );
}