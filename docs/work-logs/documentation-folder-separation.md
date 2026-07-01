# 작업 기록 문서 분리 작업 기록

## 설명

사용자는 작업에 대해 문서화하는 파일이 컨벤션 문서와 섞이고 있으므로 별도의 폴더에서 관리하라고 지시했다.

이번 작업에서는 작업 기록 성격의 문서를 `docs/work-logs/` 폴더로 이동하고, 앞으로의 문서화 기준을 명시했다.

작업한 내용은 다음과 같다.

1. `docs/work-logs/` 폴더를 만들었다.
2. `docs/cpp-local-ai-chat-ollama.md`를 `docs/work-logs/cpp-local-ai-chat-ollama.md`로 이동했다.
3. `docs/prompt-review-workflow.md`를 `docs/work-logs/prompt-review-workflow.md`로 이동했다.
4. 이동한 문서 내부의 이전 경로 참조를 새 경로로 수정했다.
5. `README.md`의 작업 기록 링크를 새 경로로 수정했다.
6. `AGENTS.md`에 작업 기록 폴더 규칙을 추가했다.
7. `docs/assistant-workflow.md`에 작업 기록 폴더 규칙을 추가했다.

## 핵심 정리

- 컨벤션 문서와 작업 기록 문서를 분리했다.
- 앞으로 개별 작업 기록은 `docs/work-logs/` 아래에 저장한다.
- `docs` 루트에는 프로젝트 기준, 컨벤션, 운영 규칙처럼 계속 참조되는 문서만 둔다.
- 다음에는 `docs/index.md`를 만들어 컨벤션 문서와 작업 기록 문서를 구분해서 링크할 수 있다.
