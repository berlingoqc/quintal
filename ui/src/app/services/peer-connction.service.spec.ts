import { TestBed } from '@angular/core/testing';

import { PeerConnctionService } from './peer-connction.service';

describe('PeerConnctionService', () => {
  let service: PeerConnctionService;

  beforeEach(() => {
    TestBed.configureTestingModule({});
    service = TestBed.inject(PeerConnctionService);
  });

  it('should be created', () => {
    expect(service).toBeTruthy();
  });
});
